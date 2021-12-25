#ifndef GATES_LOGICGATES_HPP
#define GATES_LOGICGATES_HPP

#include "Engine/Texture.hpp"
#include "pch.hpp"
#include "Gates/LogicGate.hpp"

namespace Gates {
  class NotGate : public LogicGate {
   public:
    NotGate();
    void        UpdateState();
    void        Draw();
    const char* getName() { return "not"; }
  };

  class AndGate : public LogicGate {
   public:
    AndGate();
    void        UpdateState();
    void        Draw();
    const char* getName() { return "and"; }
  };

  class OrGate : public LogicGate {
   public:
    OrGate();
    void        UpdateState();
    void        Draw();
    const char* getName() { return "or"; }
  };

  class XorGate : public LogicGate {
   public:
    XorGate();
    void        UpdateState();
    void        Draw();
    const char* getName() { return "xor"; }
  };

  class InputGate : public LogicGate {
   public:
    InputGate();
    ~InputGate();

    void        UpdateState();
    void        Draw();
    const char* getName() { return "input"; }

   private:
    static Texture err_tex;
    static Texture on_tex;
    static Texture off_tex;

    static uint64_t count;
  };

  class OutputGate : public LogicGate {
   public:
    OutputGate();
    ~OutputGate();

    void        UpdateState();
    void        Draw();
    const char* getName() { return "output"; }

   private:
    static Texture err_tex;
    static Texture on_tex;
    static Texture off_tex;

    static uint64_t count;
  };
}

#endif
