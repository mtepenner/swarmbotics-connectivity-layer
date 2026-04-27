#include <atomic>
#include <chrono>
#include <csignal>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

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

std::string toHex(const std::string& input) {
  std::ostringstream stream;
  stream << std::hex << std::setfill('0');
  for (unsigned char value : input) {
    stream << std::setw(2) << static_cast<int>(value);
  }
  return stream.str();
}

void handleSignal(int) {
  keep_running = false;
}

}  // namespace

int main(int argc, char** argv) {
  std::signal(SIGINT, handleSignal);
  const bool run_once = hasFlag(argc, argv, "--once");
  const std::vector<std::string> telemetry_messages{
      "{\"vehicle\":\"alpha\",\"battery\":81}",
      "{\"vehicle\":\"alpha\",\"latency_ms\":24}",
      "{\"vehicle\":\"alpha\",\"mission\":\"escort\"}",
  };

  do {
    std::cout << "{\"component\":\"udp_telemetry_mux\",\"frames\":[";
    for (std::size_t index = 0; index < telemetry_messages.size(); ++index) {
      const auto& message = telemetry_messages[index];
      std::cout << "{\"bytes\":" << message.size() << ",\"payload_hex\":\"" << toHex(message) << "\"}";
      if (index + 1 != telemetry_messages.size()) {
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
