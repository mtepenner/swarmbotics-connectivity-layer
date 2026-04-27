#include <algorithm>
#include <atomic>
#include <chrono>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

namespace {

std::atomic<bool> keep_running{true};

bool hasFlag(int argc, char** argv, const std::string& flag) {
  for (int index = 1; index < argc; ++index) {
    if (flag == argv[index]) {
      return true;
    }
  }
  return false;
}

double readEnvironmentDouble(const char* name, double fallback) {
  const char* raw_value = std::getenv(name);
  if (raw_value == nullptr) {
    return fallback;
  }

  try {
    return std::stod(raw_value);
  } catch (...) {
    return fallback;
  }
}

void handleSignal(int) {
  keep_running = false;
}

}  // namespace

int main(int argc, char** argv) {
  std::signal(SIGINT, handleSignal);
  const bool run_once = hasFlag(argc, argv, "--once");

  const double bandwidth_mbps = readEnvironmentDouble("SWARMBOTICS_BANDWIDTH_MBPS", 18.0);
  const double packet_loss = readEnvironmentDouble("SWARMBOTICS_PACKET_LOSS", 0.03);

  do {
    const double telemetry_share = std::clamp(0.30 + packet_loss * 2.0, 0.35, 0.75);
    const double video_share = 1.0 - telemetry_share;
    const std::string profile = bandwidth_mbps < 10.0 ? "degraded" : "balanced";

    std::cout << "{\"component\":\"traffic_shaper\",\"profile\":\"" << profile
              << "\",\"bandwidth_mbps\":" << bandwidth_mbps << ",\"telemetry_share\":" << telemetry_share
              << ",\"video_share\":" << video_share << "}" << std::endl;

    if (run_once) {
      break;
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));
  } while (keep_running);

  return 0;
}
