cmake_minimum_required(VERSION 3.16)
project(swarmbotics_connectivity_layer)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(link_quality_monitor routing-manager/src/link-quality-monitor.cpp)
add_executable(traffic_shaper routing-manager/src/traffic-shaper.cpp)
add_executable(mesh_topology_tracker radio-interfaces/manet-mesh/mesh-topology-tracker.cpp)
add_executable(udp_telemetry_mux streaming-pipeline/media-encoding/udp-telemetry-mux.cpp)
add_executable(custom_hole_punching security-layer/nat-traversal/custom-hole-punching.cpp)
