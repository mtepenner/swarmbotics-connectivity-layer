#!/usr/bin/env python3

import argparse
import json
import time


RESPONSES = {
    "AT": "OK",
    "AT+CGATT?": "+CGATT: 1",
    "AT+CSQ": "+CSQ: 22,99",
    "AT+CREG?": "+CREG: 0,1",
}


def execute(command: str) -> dict:
    return {
        "component": "modem_controller",
        "command": command,
        "response": RESPONSES.get(command, "ERROR"),
    }


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Simulate a 5G modem AT-command controller.")
    parser.add_argument("--command", default="AT+CSQ")
    parser.add_argument("--once", action="store_true")
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    while True:
        print(json.dumps(execute(args.command)), flush=True)
        if args.once:
            return 0
        time.sleep(2)


if __name__ == "__main__":
    raise SystemExit(main())
