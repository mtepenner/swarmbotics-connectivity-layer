# Swarmbotics Connectivity Layer

`swarmbotics-connectivity-layer` is the networking control plane for contested swarm operations. It models failover across MANET, 5G, and LEO SATCOM, applies traffic-shaping rules for telemetry versus video, exposes a lightweight signaling service, and stages WireGuard and IDS assets for a zero-trust deployment baseline.

## Components

- `routing-manager/`: Link-quality monitoring, routing policy evaluation, and bandwidth-aware traffic shaping.
- `radio-interfaces/`: Simulated control surfaces for tactical MANET radios, 5G modems, and LEO SATCOM terminals.
- `streaming-pipeline/`: Signaling, media pipeline generation, telemetry multiplexing, and adaptive bitrate decisions.
- `security-layer/`: WireGuard provisioning, key rotation, IDS rules, and NAT traversal planning.

## Build

Compile the C++ utilities with CMake:

```bash
cmake -S . -B build
cmake --build build
```

Python scripts use only the standard library, so `pip install -r requirements.txt` is a no-op placeholder for future dependencies.

## Validation

Run lightweight validation without external hardware:

```bash
make validate
make smoke
```

`make validate` checks Python syntax, Node syntax, and shell syntax. `make smoke` exercises the routing policy evaluator, adaptive bitrate logic, and key rotation daemon once.

## Usage

Start the failover controller:

```bash
python3 routing-manager/scripts/failover-daemon.py --config routing-manager/config/routing-policy.yaml
```

Start the signaling service:

```bash
node streaming-pipeline/webrtc-stack/signaling-server.js
```

Preview the generated GStreamer pipeline:

```bash
bash streaming-pipeline/media-encoding/gstreamer-h265-hw.sh
```

Render a WireGuard peer configuration and rotation snapshot:

```bash
bash security-layer/vpn-tunnels/wireguard-setup.sh
python3 security-layer/vpn-tunnels/key-rotation-daemon.py --once
```

## License

This project is licensed under the MIT License. See `LICENSE` for details.

