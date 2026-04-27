#!/usr/bin/env python3

import argparse
import json
import time
from pathlib import Path


DEFAULT_LINKS = {
    "manet": {"latency_ms": 24.0, "packet_loss": 0.02, "jitter_ms": 5.0},
    "5g": {"latency_ms": 38.0, "packet_loss": 0.01, "jitter_ms": 8.0},
    "satcom": {"latency_ms": 72.0, "packet_loss": 0.04, "jitter_ms": 15.0},
}


def load_policy(path: Path) -> dict:
    with path.open("r", encoding="utf-8") as handle:
        return json.load(handle)


def score_link(metrics: dict) -> float:
    return max(0.0, 100.0 - metrics["latency_ms"] * 0.25 - metrics["packet_loss"] * 200.0 - metrics["jitter_ms"] * 0.4)


def choose_link(policy: dict, links: dict) -> dict:
    candidates = []
    for name, metrics in links.items():
        link_policy = policy["links"][name]
        score = score_link(metrics)
        if score >= link_policy["min_score"] and metrics["packet_loss"] <= policy["failover"]["packet_loss_ceiling"]:
            candidates.append((score, -link_policy["priority"], name))

    if not candidates:
        fallback = min(policy["links"].items(), key=lambda item: item[1]["priority"])
        return {"selected": fallback[0], "reason": "no_link_met_thresholds", "score": score_link(links[fallback[0]])}

    best_score, _, selected = max(candidates)
    return {"selected": selected, "reason": "best_viable_score", "score": best_score}


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Evaluate failover policy across MANET, 5G, and SATCOM links.")
    parser.add_argument("--config", type=Path, required=True)
    parser.add_argument("--once", action="store_true")
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    policy = load_policy(args.config)

    while True:
        decision = choose_link(policy, DEFAULT_LINKS)
        print(
            json.dumps(
                {
                    "component": "failover_daemon",
                    "decision": decision,
                    "cooldown_seconds": policy["failover"]["cooldown_seconds"],
                    "links": DEFAULT_LINKS,
                }
            ),
            flush=True,
        )

        if args.once:
            return 0

        time.sleep(policy["failover"]["cooldown_seconds"])


if __name__ == "__main__":
    raise SystemExit(main())
