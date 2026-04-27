#include <atomic>
#include <chrono>
#include <csignal>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

namespace {

struct Node {
  std::string name;
  int neighbors;
  int hop_count;
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
  const std::vector<Node> nodes{{"alpha", 4, 0}, {"bravo", 3, 1}, {"charlie", 2, 2}, {"delta", 1, 2}};

  do {
    std::cout << "{\"component\":\"mesh_topology_tracker\",\"nodes\":[";
    for (std::size_t index = 0; index < nodes.size(); ++index) {
      const auto& node = nodes[index];
      std::cout << "{\"name\":\"" << node.name << "\",\"neighbors\":" << node.neighbors
                << ",\"hop_count\":" << node.hop_count << "}";
      if (index + 1 != nodes.size()) {
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
