#ifndef SIMX_LOGGER_HPP
#define SIMX_LOGGER_HPP

#include "pch.hpp"

namespace SimX {
  class Logger {
   public:
    static void Info(const std::string_view message);
    static void Die [[noreturn]] (const std::string_view message, int exit_code = -1);
  };
}

#endif
