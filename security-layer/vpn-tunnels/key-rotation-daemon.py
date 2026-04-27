#!/usr/bin/env python3

import argparse
import json
import secrets
import time


def rotate() -> dict:
    return {
        "component": "key_rotation_daemon",
        "active_key_id": secrets.token_hex(8),
        "next_key_id": secrets.token_hex(8),
        "rotation_window_seconds": 900,
    }


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Generate a WireGuard key rotation snapshot.")
    parser.add_argument("--once", action="store_true")
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    while True:
        print(json.dumps(rotate()), flush=True)
        if args.once:
            return 0
        time.sleep(15)


if __name__ == "__main__":
    raise SystemExit(main())
