PYTHON ?= python

.PHONY: help validate python-check node-check shell-check smoke

help:
	@echo "Targets: validate, smoke"

validate: python-check node-check shell-check

python-check:
	$(PYTHON) -m compileall routing-manager radio-interfaces streaming-pipeline security-layer

node-check:
	node --check streaming-pipeline/webrtc-stack/signaling-server.js

shell-check:
	bash -n streaming-pipeline/media-encoding/gstreamer-h265-hw.sh
	bash -n security-layer/vpn-tunnels/wireguard-setup.sh

smoke:
	$(PYTHON) routing-manager/scripts/failover-daemon.py --once --config routing-manager/config/routing-policy.yaml
	$(PYTHON) streaming-pipeline/qos-manager/adaptive-bitrate.py --once
	$(PYTHON) security-layer/vpn-tunnels/key-rotation-daemon.py --once
