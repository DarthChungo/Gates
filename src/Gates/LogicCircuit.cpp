#include "Engine/Renderer.hpp"
#include "Gates/LogicCircuit.hpp"
#include "Gates/LogicGates.hpp"
#include "Util/Misc.hpp"

namespace Gates {
  void LogicCircuit::UpdateState() {
    std::set<std::shared_ptr<LogicGate>> gates_updating;

    while (!gates_update_pending.empty()) {
      gates_updating = gates_update_pending;
      gates_update_pending.clear();

      for (auto&& gate : gates_updating) {
        State previous_state = gate->state;
        gate->UpdateState();

        if (auto it = gates_update_forced.find(gate); it != gates_update_forced.end()) {
          for (auto&& child : gate->outputs) {
            gates_update_pending.insert(child);
            gates_update_forced.erase(it);
          }
        } else if (gate->state != previous_state) {
          for (auto&& child : gate->outputs) {
            gates_update_pending.insert(child);
          }
        }
      }
    }
  }

  void LogicCircuit::DrawGates(const glm::vec2& mouse_pos, const Button& left_click) {
    for (auto&& gate : gates) {
      switch (gate->state) {
        case State::ERROR:
          Renderer::DrawQuad(gate->pos, {10.f, 10.f}, {1.f, 0.f, 0.f, 1.f});
          break;

        case State::ON:
          Renderer::DrawQuad(gate->pos, {10.f, 10.f}, {0.f, 1.f, 0.f, 1.f});
          break;

        case State::OFF:
          Renderer::DrawQuad(gate->pos, {10.f, 10.f}, {3.f, 3.f, 3.f, 1.f});
          break;
      }
    }
  }

  void LogicCircuit::SetInput(const std::shared_ptr<LogicGate>& input, State val) {
    input->state = val;
    gates_update_pending.insert(input);
    gates_update_forced.insert(input);
  }

  void LogicCircuit::AddOutput(const std::shared_ptr<LogicGate>& who, const std::shared_ptr<LogicGate>& output) {
    who->outputs.insert(output);
    output->inputs.insert(who);
  }
}
