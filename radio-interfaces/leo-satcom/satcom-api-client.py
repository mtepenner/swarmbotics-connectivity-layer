#!/usr/bin/env python3

import argparse
import json
import time


def build_status(downlink_mbps: float, obstruction_pct: float) -> dict:
    return {
        "component": "satcom_api_client",
        "downlink_mbps": downlink_mbps,
        "uplink_mbps": round(downlink_mbps / 4.0, 2),
        "obstruction_pct": obstruction_pct,
        "state": "healthy" if obstruction_pct < 10 else "degraded",
    }


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Simulate polling a LEO SATCOM terminal for health.")
    parser.add_argument("--downlink", type=float, default=72.0)
    parser.add_argument("--obstruction", type=float, default=4.0)
    parser.add_argument("--once", action="store_true")
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    while True:
        print(json.dumps(build_status(args.downlink, args.obstruction)), flush=True)
        if args.once:
            return 0
        time.sleep(2)


if __name__ == "__main__":
    raise SystemExit(main())
