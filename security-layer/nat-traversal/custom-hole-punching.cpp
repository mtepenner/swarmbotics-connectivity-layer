#include <atomic>
#include <chrono>
#include <csignal>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

namespace {

struct PunchStep {
  std::string phase;
  int local_port;
  int remote_port;
};

std::atomic<bool> keep_running{true};

bool hasFlag(int argc, char** argv, const std::string& flag) {
  for (int index = 1; index < argc; ++index) {
    if (flag == argv[index]) {
      return true;
    }
  }
  return false;
}

void handleSignal(int) {
  keep_running = false;
}

}  // namespace

int main(int argc, char** argv) {
  std::signal(SIGINT, handleSignal);
  const bool run_once = hasFlag(argc, argv, "--once");
  const std::vector<PunchStep> steps{{"rendezvous", 45000, 45010}, {"simultaneous_open", 45001, 45011}, {"keepalive", 45002, 45012}};

  do {
    std::cout << "{\"component\":\"custom_hole_punching\",\"steps\":[";
    for (std::size_t index = 0; index < steps.size(); ++index) {
      const auto& step = steps[index];
      std::cout << "{\"phase\":\"" << step.phase << "\",\"local_port\":" << step.local_port
                << ",\"remote_port\":" << step.remote_port << "}";
      if (index + 1 != steps.size()) {
        std::cout << ',';
      }
    }
    std::cout << "]}" << std::endl;

    if (run_once) {
      break;
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));
  } while (keep_running);

  return 0;
}