#!/usr/bin/env python3

import argparse
import json
import time


def choose_profile(bandwidth_mbps: float, packet_loss: float) -> dict:
    if bandwidth_mbps < 6 or packet_loss > 0.09:
        return {"profile": "failsafe", "resolution": "854x480", "bitrate_kbps": 1200}
    if bandwidth_mbps < 12 or packet_loss > 0.05:
        return {"profile": "degraded", "resolution": "1280x720", "bitrate_kbps": 2200}
    return {"profile": "mission", "resolution": "1920x1080", "bitrate_kbps": 3500}


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Compute an adaptive bitrate decision from current network conditions.")
    parser.add_argument("--bandwidth", type=float, default=14.0)
    parser.add_argument("--packet-loss", type=float, default=0.03)
    parser.add_argument("--once", action="store_true")
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    while True:
        profile = choose_profile(args.bandwidth, args.packet_loss)
        print(
            json.dumps(
                {
                    "component": "adaptive_bitrate",
                    "bandwidth_mbps": args.bandwidth,
                    "packet_loss": args.packet_loss,
                    **profile,
                }
            ),
            flush=True,
        )
        if args.once:
            return 0
        time.sleep(2)


if __name__ == "__main__":
    raise SystemExit(main())
