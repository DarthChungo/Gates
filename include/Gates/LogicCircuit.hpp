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

#ifndef GATES_LOGICCIRCUIT_HPP
#define GATES_LOGICCIRCUIT_HPP

#include "Gates/include.hpp"
#include "Gates/LogicGates.hpp"
#include "Gates/LogicGate.hpp"

namespace Gates {
  class GatesApplication;

  class LogicCircuit {
   public:
    using TruthTableEntry = std::pair<std::vector<bool>, std::vector<bool>>;
    using TruthTable      = std::vector<TruthTableEntry>;

    LogicCircuit(GatesApplication* application) : app(application) {}

    template <class GateType>
    std::shared_ptr<LogicGate> AddGate() requires std::derived_from<GateType, LogicGate> {
      auto new_gate = std::make_shared<GateType>();
      gates.insert(new_gate);

      adding      = true;
      adding_gate = new_gate;

      if constexpr (std::is_same<GateType, InputGate>::value) {
        gates_input.insert(new_gate);
        new_gate->state = State::OFF;
      } else if constexpr (std::is_same<GateType, OutputGate>::value) {
        gates_output.insert(new_gate);
      }

      return new_gate;
    }

    template <class GateType>
    std::shared_ptr<LogicGate> AddGateDirect(const glm::vec2 pos,
                                             const px::UUID& uuid) requires std::derived_from<GateType, LogicGate> {
      auto new_gate = std::make_shared<GateType>(uuid);
      new_gate->pos = pos;
      gates.insert(new_gate);

      if constexpr (std::is_same<GateType, InputGate>::value) {
        gates_input.insert(new_gate);
        new_gate->state = State::OFF;
      } else if constexpr (std::is_same<GateType, OutputGate>::value) {
        gates_output.insert(new_gate);
      }

      return new_gate;
    }

    void UpdateLogicState();
    void UpdateGraphicsState();
    void DrawGates();

    void SetInput(const std::shared_ptr<LogicGate>& input, State val);
    void ToggleConnection(const std::shared_ptr<LogicGate>& from, const std::shared_ptr<LogicGate>& to);
    void MakeConnection(const px::UUID& from, const px::UUID& to);

    TruthTable ComputeTruthTable();

   public:
    std::set<std::shared_ptr<LogicGate>> gates;
    std::set<std::shared_ptr<LogicGate>> gates_update_pending;
    std::set<std::shared_ptr<LogicGate>> gates_update_forced;
    std::set<std::shared_ptr<LogicGate>> gates_input;
    std::set<std::shared_ptr<LogicGate>> gates_output;

    std::shared_ptr<LogicGate> hovered_gate;
    std::shared_ptr<LogicGate> selected_gate;
    std::shared_ptr<LogicGate> dragging_gate;
    glm::vec2                  dragging_offset = {};
    bool                       dragging        = false;

    std::shared_ptr<LogicGate> adding_gate;
    bool                       adding = false;

    std::shared_ptr<LogicGate> outputing_gate;
    bool                       outputing = false;

    GatesApplication* const app;
  };
}

#endif
