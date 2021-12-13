#ifndef GATES_LOGICCIRCUIT_HPP
#define GATES_LOGICCIRCUIT_HPP

#include "pch.hpp"
#include "Gates/LogicGate.hpp"

namespace Gates {
  class LogicCircuit {
   public:
    void UpdateState();

   private:
    std::vector<LogicGate> gates;
  };
}

#endif
