#ifndef GATES_LOGICGATES_HPP
#define GATES_LOGICGATES_HPP

#include "Pixel/Pixel.hpp"
#include "Gates/LogicGate.hpp"

namespace px = Pixel;

namespace Gates {
  // Declaraciones de todas las puertas

  class NotGate : public LogicGate {
   public:
    NotGate();
    NotGate(const px::UUID& uuid);

    void        UpdateState();
    void        Draw();
    const char* getName() { return "not"; }
  };

  class AndGate : public LogicGate {
   public:
    AndGate();
    AndGate(const px::UUID& uuid);

    void        UpdateState();
    void        Draw();
    const char* getName() { return "and"; }
  };

  class OrGate : public LogicGate {
   public:
    OrGate();
    OrGate(const px::UUID& uuid);

    void        UpdateState();
    void        Draw();
    const char* getName() { return "or"; }
  };

  class XorGate : public LogicGate {
   public:
    XorGate();
    XorGate(const px::UUID& uuid);

    void        UpdateState();
    void        Draw();
    const char* getName() { return "xor"; }
  };

  class InputGate : public LogicGate {
   public:
    InputGate();
    InputGate(const px::UUID& uuid);

    void        UpdateState();
    void        Draw();
    const char* getName() { return "input"; }
  };

  class OutputGate : public LogicGate {
   public:
    OutputGate();
    OutputGate(const px::UUID& uuid);

    void        UpdateState();
    void        Draw();
    const char* getName() { return "output"; }
  };
}

#endif
