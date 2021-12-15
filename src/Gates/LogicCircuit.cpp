#include "Engine/Renderer.hpp"
#include "Gates/GatesApplication.hpp"
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

  bool inline operator>(const glm::vec2& left, const glm::vec2& right) {
    return (left.x > right.x) && (left.y > right.y);
  }
  bool inline operator<(const glm::vec2& left, const glm::vec2& right) {
    return (left.x < right.x) && (left.y < right.y);
  }

  void LogicCircuit::DrawGates(const Mouse& mouse) {
    for (auto&& gate : gates) {
      if (gate->pos > (app->MousePosWorld() - glm::vec2 {10.f, 10.f}) && gate->pos < app->MousePosWorld() &&
          mouse.button(MouseButton::BUTTON_1).pressed) {
        if (app->KeyboardKey(KeyboardKey::KEY_LEFT_ALT).held && ! instanceof <OutputGate>(gate.get())) {
          outputing_gate = gate;
          outputing      = true;

        } else {
          dragging_offset = app->MousePosWorld() - gate->pos;
          dragging_gate   = gate;
          dragging        = true;
        }
      }

      if (mouse.button(MouseButton::BUTTON_1).released) {
        dragging = false;

        if (outputing) {
          std::shared_ptr<LogicGate> potential_output;

          for (auto&& output : gates) {
            if (output == gate) continue;
            if (output->pos > (app->MousePosWorld() - glm::vec2 {10.f, 10.f}) && output->pos < app->MousePosWorld()) {
              potential_output = output;
            }
          }

          if (potential_output && ! instanceof <InputGate>(potential_output.get()))
            MakeConnection(outputing_gate, potential_output);

          outputing = false;
          outputing_gate.reset();
        }
      }

      if (mouse.button(MouseButton::BUTTON_1).pressed && adding) {
        adding = false;
        adding_gate.reset();
      }

      if (dragging && gate == dragging_gate) {
        gate->pos = app->MousePosWorld() - dragging_offset;

      } else if (adding && gate == adding_gate) {
        gate->pos = app->MousePosWorld() - glm::vec2 {5.f, 5.f};

      } else if (outputing && gate == outputing_gate) {
        Renderer::DrawLine(gate->pos + glm::vec2 {10.f, 5.f}, app->MousePosWorld());
      }

      switch (gate->state) {
        case State::ERROR:
          Renderer::DrawQuad(gate->pos, {10.f, 10.f}, {1.f, 0.f, 0.f, 1.f});
          break;

        case State::ON:
          Renderer::DrawQuad(gate->pos, {10.f, 10.f}, {0.f, 1.f, 0.f, 1.f});
          break;

        case State::OFF:
          Renderer::DrawQuad(gate->pos, {10.f, 10.f}, {0.3f, 0.3f, 0.3f, 1.f});
          break;
      }

      for (auto&& output : gate->outputs) {
        switch (gate->state) {
          case State::ERROR:
            Renderer::DrawLine(
                gate->pos + glm::vec2 {10.f, 5.f}, output->pos + glm::vec2 {0.f, 5.f}, {1.f, 0.f, 0.f, 1.f});
            break;

          case State::ON:
            Renderer::DrawLine(
                gate->pos + glm::vec2 {10.f, 5.f}, output->pos + glm::vec2 {0.f, 5.f}, {0.f, 1.f, 0.f, 1.f});
            break;

          case State::OFF:
            Renderer::DrawLine(
                gate->pos + glm::vec2 {10.f, 5.f}, output->pos + glm::vec2 {0.f, 5.f}, {0.3f, 0.3f, 0.3f, 1.f});
            break;
        }
      }
    }
  }

  void LogicCircuit::SetInput(const std::shared_ptr<LogicGate>& input, State val) {
    input->state = val;
    gates_update_pending.insert(input);
    gates_update_forced.insert(input);
  }

  void LogicCircuit::MakeConnection(const std::shared_ptr<LogicGate>& who, const std::shared_ptr<LogicGate>& output) {
    who->outputs.insert(output);
    output->inputs.insert(who);

    gates_update_pending.insert(output);
  }
}
