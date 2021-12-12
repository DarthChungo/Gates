#include "Util/Logger.hpp"

namespace SimX {
  void Logger::Info(const std::string_view message) { std::cout << "[INFO] " << message << "\n"; }

  void Logger::Die(const std::string_view message, int exit_code) {
    std::cout << "[FATAL ERROR] " << message << std::endl;
    exit(exit_code);
  }
}
