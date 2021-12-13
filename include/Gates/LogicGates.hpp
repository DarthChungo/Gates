#ifndef GATES_LOGICGATES_HPP
#define GATES_LOGICGATES_HPP

#include "pch.hpp"
#include "Gates/LogicGate.hpp"

namespace Gates {
  class NotGate : public LogicGate {
    void        UpdateState();
    void        DrawGate();
    const char* getName() { return "not"; }
  };

  class AndGate : public LogicGate {
    void        UpdateState();
    void        DrawGate();
    const char* getName() { return "and"; }
  };

  class OrGate : public LogicGate {
    void        UpdateState();
    void        DrawGate();
    const char* getName() { return "or"; }
  };

  class XorGate : public LogicGate {
    void        UpdateState();
    void        DrawGate();
    const char* getName() { return "xor"; }
  };

  class InputGate : public LogicGate {
    void        UpdateState();
    void        DrawGate();
    const char* getName() { return "input"; }
  };

  class OutputGate : public LogicGate {
    void        UpdateState();
    void        DrawGate();
    const char* getName() { return "output"; }
  };
}

#endif
