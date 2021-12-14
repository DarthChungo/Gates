#ifndef GATES_LOGICCIRCUIT_HPP
#define GATES_LOGICCIRCUIT_HPP

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
      gates_update_pending.insert(new_gate);
      return new_gate;
    }

    void UpdateState();
    void DrawGates(const Mouse& mouse);

    void SetInput(const std::shared_ptr<LogicGate>& input, State val);
    void AddOutput(const std::shared_ptr<LogicGate>& who, const std::shared_ptr<LogicGate>& output);

   public:
    std::set<std::shared_ptr<LogicGate>> gates;
    std::set<std::shared_ptr<LogicGate>> gates_update_pending;
    std::set<std::shared_ptr<LogicGate>> gates_update_forced;

    std::shared_ptr<LogicGate> dragging_gate;
    glm::vec2                  dragging_pos = {};
    bool                       dragging     = false;

    GatesApplication* const app;
  };
}

#endif
