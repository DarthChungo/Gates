#ifndef GATES_LOGICGATES_HPP
#define GATES_LOGICGATES_HPP

#include "pch.hpp"
#include "Gates/LogicGate.hpp"

namespace Gates {
  class NotGate : private LogicGate {
    void UpdateState();
    void DrawGate();
  };

  class AndGate : private LogicGate {
    void UpdateState();
    void DrawGate();
  };

  class OrGate : private LogicGate {
    void UpdateState();
    void DrawGate();
  };

  class XorGate : private LogicGate {
    void UpdateState();
    void DrawGate();
  };

  class InputGate : private LogicGate {
    void UpdateState();
    void DrawGate();
  };

  class OutputGate : private LogicGate {
    void UpdateState();
    void DrawGate();
  };
}

#endif
