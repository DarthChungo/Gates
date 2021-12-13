#ifndef GATES_LOGICGATE_HPP
#define GATES_LOGICGATE_HPP

#include "pch.hpp"

namespace Gates {
  enum class State : uint32_t { ON, OFF, ERROR };

  class LogicGate {
   public:
    virtual void UpdateState() = 0;
    virtual void DrawGate()    = 0;

   protected:
    std::vector<State>      inputs;
    std::vector<LogicGate*> outputs;
    State                   state = State::ERROR;
  };
}

#endif
