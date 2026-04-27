#include <algorithm>
#include <atomic>
#include <chrono>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

namespace {

struct LinkSample {
  std::string name;
  double latency_ms;
  double packet_loss;
  double jitter_ms;
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

double scoreLink(const LinkSample& sample) {
  const double raw_score = 100.0 - sample.latency_ms * 0.25 - sample.packet_loss * 200.0 - sample.jitter_ms * 0.4;
  return std::clamp(raw_score, 0.0, 100.0);
}

void handleSignal(int) {
  keep_running = false;
}

}  // namespace

int main(int argc, char** argv) {
  std::signal(SIGINT, handleSignal);
  const bool run_once = hasFlag(argc, argv, "--once");

  const std::vector<LinkSample> samples{
      {"manet", 24.0, 0.02, 5.0},
      {"5g", 38.0, 0.01, 8.0},
      {"satcom", 72.0, 0.04, 15.0},
  };

  do {
    std::string preferred_link = "manet";
    double best_score = -1.0;

    std::cout << "{\"component\":\"link_quality_monitor\",\"links\":[";
    for (std::size_t index = 0; index < samples.size(); ++index) {
      const auto& sample = samples[index];
      const double link_score = scoreLink(sample);
      if (link_score > best_score) {
        best_score = link_score;
        preferred_link = sample.name;
      }

      std::cout << "{\"name\":\"" << sample.name << "\",\"latency_ms\":" << sample.latency_ms
                << ",\"packet_loss\":" << sample.packet_loss << ",\"jitter_ms\":" << sample.jitter_ms
                << ",\"score\":" << link_score << "}";
      if (index + 1 != samples.size()) {
        std::cout << ',';
      }
    }
    std::cout << "],\"preferred_link\":\"" << preferred_link << "\"}" << std::endl;

    if (run_once) {
      break;
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));
  } while (keep_running);

  return 0;
}
