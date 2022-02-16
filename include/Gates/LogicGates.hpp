/*
Gates, a simple logic circuit simulator written in C++
Copyright (C) 2022 DarthChungo

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef GATES_LOGICGATES_HPP
#define GATES_LOGICGATES_HPP

#include "Gates/include.hpp"
#include "Gates/LogicGate.hpp"

namespace Gates {
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
