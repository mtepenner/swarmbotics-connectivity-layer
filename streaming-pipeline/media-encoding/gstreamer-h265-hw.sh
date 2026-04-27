#!/usr/bin/env bash
set -euo pipefail

GST_COMMAND=(
  gst-launch-1.0
  v4l2src
  io-mode=dmabuf
  '!'
  videoconvert
  '!'
  x265enc
  tune=zerolatency
  bitrate="${SWARMBOTICS_BITRATE_KBPS:-3500}"
  '!'
  h265parse
  '!'
  rtph265pay
  pt=96
  config-interval=1
  '!'
  udpsink
  host="${SWARMBOTICS_STREAM_HOST:-127.0.0.1}"
  port="${SWARMBOTICS_STREAM_PORT:-5600}"
)

if [[ "${GST_EXECUTE:-0}" == "1" ]]; then
  exec "${GST_COMMAND[@]}"
fi

printf '%s\n' "${GST_COMMAND[*]}"
