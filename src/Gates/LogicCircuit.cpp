#include "Engine/Renderer.hpp"
#include "Gates/GatesApplication.hpp"
#include "Gates/LogicCircuit.hpp"
#include "Gates/LogicGates.hpp"
#include "Util/Misc.hpp"

namespace Gates {
  void LogicCircuit::UpdateLogicState() {
    std::set<std::shared_ptr<LogicGate>> gates_updating;

    while (!gates_update_pending.empty()) {
      gates_updating = gates_update_pending;
      gates_update_pending.clear();

      for (auto&& gate : gates_updating) {
        State previous_state = gate->state;
        gate->UpdateState();

        if (auto it = gates_update_forced.find(gate); it != gates_update_forced.end()) {
          gates_update_forced.erase(it);

          for (auto&& child : gate->outputs) {
            gates_update_pending.insert(child);
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

  void LogicCircuit::UpdateGraphicsState() {
    hovered_gate.reset();

    for (auto&& gate : gates) {
      if (gate->pos > (app->MousePosWorld() - gate->size) && gate->pos < app->MousePosWorld()) {
        if (app->MouseButton(MouseButton::BUTTON_1).pressed) {
          if (app->KeyboardKey(KeyboardKey::KEY_LEFT_ALT).held && ! instanceof <OutputGate>(gate.get())) {
            outputing_gate = gate;
            outputing      = true;

          } else if (app->KeyboardKey(KeyboardKey::KEY_LEFT_SHIFT).held && instanceof <InputGate>(gate.get())) {
            SetInput(gate, (gate->state == State::OFF || gate->state == State::ERROR) ? State::ON : State::OFF);

          } else {
            selected_gate = gate;

            dragging_offset = app->MousePosWorld() - gate->pos;
            dragging_gate   = gate;
            dragging        = true;
          }
        } else if (app->MouseButton(MouseButton::BUTTON_1).released && outputing) {
          if (gate != outputing_gate && ! instanceof <InputGate>(gate.get())) {
            ToggleConnection(outputing_gate, gate);
          }

          outputing = false;
          outputing_gate.reset();
        }

        hovered_gate = gate;

      } else if (app->MouseButton(MouseButton::BUTTON_1).pressed && (selected_gate == gate)) {
        selected_gate.reset();
      }
    }

    if (app->KeyboardKey(KeyboardKey::KEY_DELETE).pressed && selected_gate) {
      gates.erase(selected_gate);
      gates_input.erase(selected_gate);
      gates_output.erase(selected_gate);

      dragging_gate.reset();
      dragging = false;

      for (auto&& output : selected_gate->outputs) {
        output->inputs.erase(selected_gate);
        gates_update_pending.insert(output);
      }

      selected_gate->outputs.clear();

      for (auto&& input : selected_gate->inputs) {
        input->outputs.erase(selected_gate);
        gates_update_pending.insert(input);
      }

      selected_gate->inputs.clear();
      selected_gate.reset();
    }

    if (app->MouseButton(MouseButton::BUTTON_1).pressed && adding) {
      adding = false;
      adding_gate.reset();

    } else if (app->MouseButton(MouseButton::BUTTON_1).released) {
      dragging = false;

      outputing = false;
      outputing_gate.reset();
    }

    if (dragging) {
      dragging_gate->pos = app->MousePosWorld() - dragging_offset;

    } else if (adding) {
      adding_gate->pos = app->MousePosWorld() - glm::vec2 {5.f, 5.f};
    }
  }

  void LogicCircuit::DrawGates() {
    for (auto&& gate : gates) {
      gate->Draw();
    }

    if (outputing) {
      Renderer::DrawLine(outputing_gate->pos + (outputing_gate->size * glm::vec2 {1.f, 0.5f}), app->MousePosWorld());
      Renderer::OutlineQuad(outputing_gate->pos, outputing_gate->size, {1.f, 0.f, 1.f, 1.f});
      if (hovered_gate) Renderer::OutlineQuad(hovered_gate->pos, hovered_gate->size, {1.f, 0.f, 1.f, 1.f});
    }

    if (selected_gate) Renderer::OutlineQuad(selected_gate->pos, selected_gate->size, {1.f, 1.f, 0.f, 1.f});
  }

  void LogicCircuit::SetInput(const std::shared_ptr<LogicGate>& input, State val) {
    input->state = val;
    gates_update_pending.insert(input);
    gates_update_forced.insert(input);
  }

  void LogicCircuit::ToggleConnection(const std::shared_ptr<LogicGate>& who, const std::shared_ptr<LogicGate>& output) {
    if (who->outputs.contains(output) || output->inputs.contains(who)) {
      who->outputs.erase(output);
      output->inputs.erase(who);

    } else {
      who->outputs.insert(output);
      output->inputs.insert(who);
    }

    gates_update_pending.insert(output);
  }
}
