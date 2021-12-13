#ifndef GATES_LOGICCIRCUIT_HPP
#define GATES_LOGICCIRCUIT_HPP

#include "pch.hpp"
#include "Gates/LogicGate.hpp"

namespace Gates {
  class LogicCircuit {
   public:
    friend class GatesApplication;

    void UpdateState();

   private:
    std::vector<std::shared_ptr<LogicGate>> gates;
  };
}

#endif
