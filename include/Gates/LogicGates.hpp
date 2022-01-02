#ifndef GATES_LOGICGATES_HPP
#define GATES_LOGICGATES_HPP

#include "Engine/Texture.hpp"
#include "pch.hpp"
#include "Gates/LogicGate.hpp"

namespace Gates {
  class NotGate : public LogicGate {
   public:
    NotGate();
    NotGate(const UUID& uuid);

    void        UpdateState();
    void        Draw();
    const char* getName() { return "not"; }
  };

  class AndGate : public LogicGate {
   public:
    AndGate();
    AndGate(const UUID& uuid);

    void        UpdateState();
    void        Draw();
    const char* getName() { return "and"; }
  };

  class OrGate : public LogicGate {
   public:
    OrGate();
    OrGate(const UUID& uuid);

    void        UpdateState();
    void        Draw();
    const char* getName() { return "or"; }
  };

  class XorGate : public LogicGate {
   public:
    XorGate();
    XorGate(const UUID& uuid);

    void        UpdateState();
    void        Draw();
    const char* getName() { return "xor"; }
  };

  class InputGate : public LogicGate {
   public:
    InputGate();
    InputGate(const UUID& uuid);

    void        UpdateState();
    void        Draw();
    const char* getName() { return "input"; }
  };

  class OutputGate : public LogicGate {
   public:
    OutputGate();
    OutputGate(const UUID& uuid);

    void        UpdateState();
    void        Draw();
    const char* getName() { return "output"; }
  };
}

#endif
