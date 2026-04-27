#!/usr/bin/env python3

import argparse
import json
import time


def build_status(channel: str, neighbors: int) -> dict:
    return {
        "component": "tactical_radio_driver",
        "channel": channel,
        "neighbors": neighbors,
        "state": "linked" if neighbors > 0 else "searching",
    }


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Simulate a tactical MANET radio status report.")
    parser.add_argument("--channel", default="mesh-alpha")
    parser.add_argument("--neighbors", type=int, default=6)
    parser.add_argument("--once", action="store_true")
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    while True:
        print(json.dumps(build_status(args.channel, args.neighbors)), flush=True)
        if args.once:
            return 0
        time.sleep(2)


if __name__ == "__main__":
    raise SystemExit(main())
