#ifndef GATES_LOGICCIRCUIT_HPP
#define GATES_LOGICCIRCUIT_HPP

#include "Gates/LogicGates.hpp"
#include "pch.hpp"
#include "Engine/Application.hpp"
#include "Gates/LogicGate.hpp"
#include "Util/UUID.hpp"

namespace Gates {
  class GatesApplication;

  class LogicCircuit {
   public:
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
    std::shared_ptr<LogicGate> AddGateDirect(const glm::vec2 pos) requires std::derived_from<GateType, LogicGate> {
      auto new_gate = std::make_shared<GateType>();
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

    void UpdateState();
    void DrawGates(const Mouse& mouse);

    void SetInput(const std::shared_ptr<LogicGate>& input, State val);
    void ToggleConnection(const std::shared_ptr<LogicGate>& who, const std::shared_ptr<LogicGate>& output);

   public:
    std::set<std::shared_ptr<LogicGate>> gates;
    std::set<std::shared_ptr<LogicGate>> gates_update_pending;
    std::set<std::shared_ptr<LogicGate>> gates_update_forced;
    std::set<std::shared_ptr<LogicGate>> gates_input;
    std::set<std::shared_ptr<LogicGate>> gates_output;

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
