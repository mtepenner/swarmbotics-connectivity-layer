#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
OUT_DIR="${SCRIPT_DIR}/out"
CONFIG_PATH="${OUT_DIR}/swarmbotics-wg0.conf"

mkdir -p "${OUT_DIR}"

cat > "${CONFIG_PATH}" <<EOF
[Interface]
Address = ${SWARMBOTICS_WG_ADDRESS:-10.88.0.2/24}
PrivateKey = ${SWARMBOTICS_WG_PRIVATE_KEY:-replace-with-private-key}
ListenPort = ${SWARMBOTICS_WG_PORT:-51820}

[Peer]
PublicKey = ${SWARMBOTICS_WG_PEER_PUBLIC_KEY:-replace-with-peer-public-key}
AllowedIPs = ${SWARMBOTICS_WG_ALLOWED_IPS:-10.88.0.0/24}
Endpoint = ${SWARMBOTICS_WG_ENDPOINT:-198.51.100.10:51820}
PersistentKeepalive = 15
EOF

printf '%s\n' "Generated WireGuard config at ${CONFIG_PATH}"

if [[ "${WG_APPLY:-0}" == "1" ]] && command -v wg-quick >/dev/null 2>&1; then
  sudo wg-quick up "${CONFIG_PATH}"
fi
