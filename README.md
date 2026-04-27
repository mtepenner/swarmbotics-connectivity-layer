# The Connectivity Layer: Swarm Networking

## Description
`swarmbotics_connectivity_layer` provides the unbreakable, secure communication backbone required for massive-scale robotic swarm operations. By leveraging triple-redundant dynamic routing across MANET, 5G, and LEO SATCOM, this module ensures that critical Command and Control (C2) telemetry and low-latency video streams reach the operator interface, even in contested or degraded network environments. 

## Table of Contents
- [Features](#-features)
- [Technologies Used](#%EF%B8%8F-technologies-used)
- [Installation](#%EF%B8%8F-installation)
- [Usage](#-usage)
- [Contributing](#-contributing)
- [License](#-license)

## 🚀 Features

* **Triple-Redundant Failover**: Seamlessly transitions between Localized P2P MANET mesh networks (e.g., Silvus, TrellisWare), Cellular 5G, and LEO SATCOM (e.g., Starlink) based on real-time link quality.
* **Intelligent Traffic Shaping**: Continuously monitors network latency and packet loss, dynamically prioritizing critical C2 telemetry over video streams during low-bandwidth scenarios.
* **Ultra-Low Latency Streaming**: Features a robust WebRTC stack and hardware-accelerated GStreamer H.265 encoding, paired with an adaptive bitrate manager to prevent connection drops.
* **Zero-Trust Security**: Employs ultra-fast WireGuard E2E encrypted tunnels with automated key rotation. Includes an Intrusion Detection System (IDS) to monitor for jamming or spoofing, and custom NAT hole punching to bypass complex firewalls.

## 🛠️ Technologies Used
* **Languages**: C++, Python, JavaScript
* **Networking**: WebRTC, UDP, WireGuard, AT Commands
* **Hardware Interfacing**: GStreamer, Tactical Radio APIs, 5G Modems
* **Security**: End-to-End Encryption, Dynamic Key Rotation

## ⚙️ Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/mtepenner/swarmbotics_connectivity_layer.git
   cd swarmbotics_connectivity_layer
   ```
2. Install the necessary system dependencies (e.g., GStreamer, WireGuard):
   ```bash
   sudo apt-get install gstreamer1.0-tools wireguard
   ```
3. Compile the C++ network driver and routing components:
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```
4. Install Python dependencies for the failover daemon and radio interfaces:
   ```bash
   pip install -r requirements.txt
   ```

## 💻 Usage

### Starting the Failover Daemon
To initiate the dynamic routing manager that handles the MANET, 5G, and SATCOM failover logic:
```bash
python3 routing_manager/scripts/failover_daemon.py --config routing_manager/config/routing_policy.yaml
```

### Initializing the Secure Tunnel
To bring up the WireGuard VPN and establish the E2E encrypted connection:
```bash
sudo bash security_layer/vpn_tunnels/wireguard_setup.sh
python3 security_layer/vpn_tunnels/key_rotation_daemon.py
```

### Launching the WebRTC Stream
Start the signaling server and hardware-accelerated video pipeline for operator teleoperation:
```bash
node streaming_pipeline/webrtc_stack/signaling_server.js
bash streaming_pipeline/media_encoding/gstreamer_h265_hw.sh
```

## 🤝 Contributing
Contributions are highly appreciated, especially in the areas of improving NAT traversal algorithms, adding support for new tactical radio hardware, or optimizing the UDP telemetry multiplexer. Please ensure that all C++ code passes local memory leak checks and conforms to the project's formatting standards before opening a pull request.

## 📄 License
This project is licensed under the [MIT License](LICENSE) - see the LICENSE file for details. Copyright (c) 2026 Matthew Penner.
