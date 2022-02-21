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

#include "Pixel/Pixel.hpp"
#include "Gates/GatesApplication.hpp"
#include "Gates/LogicCircuit.hpp"
#include "Gates/LogicGates.hpp"

namespace Gates {
  void LogicCircuit::UpdateLogicState() {
    // Este conjunto guardará referencias a las puertas lógicas que estan siendo procesadas
    std::set<std::shared_ptr<LogicGate>> gates_updating;

    // Se sigue trabajando hasta haber procesado todas las puertas pendientes
    while (!gates_update_pending.empty()) {
      // Se insertan las puertas pendientes por actualizar en el conjunto de trabajo
      gates_updating = gates_update_pending;
      gates_update_pending.clear();

      // Se iteran todas las puertas del conjunto de trabajo
      for (auto&& gate : gates_updating) {
        // Se guarda el estado previo y se actualiza utilizando la abstracción para llamar a el método apropiado
        State previous_state = gate->state;
        gate->UpdateState();

        // Si la aplicación ha indicado que la puerta debe ser acualizada si o si (p. ej. al insertar una puerta nueva
        // en el circuito), esta se actualiza
        if (auto it = gates_update_forced.find(gate); it != gates_update_forced.end()) {
          gates_update_forced.erase(it);

          // Se añaden todos los hijos de la puerta a las puertas pendientes por actualizar para propagar los cambios
          // por el circuito automáticamente
          for (auto&& child : gate->outputs) {
            gates_update_pending.insert(child);
          }

          // Si el estado de una perta ha cambiado al actualizarla, hay que propagar sus cambios
        } else if (gate->state != previous_state) {
          // Se añaden todos los hijos de la puerta a las puertas pendientes por actualizar para propagar los cambios
          // por el circuito automáticamente
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
        if (app->MouseButton(px::MouseButton::BUTTON_1).pressed) {
          if (app->KeyboardKey(px::KeyboardKey::KEY_LEFT_ALT).held && !px:: instanceof <OutputGate>(gate.get())) {
            outputing_gate = gate;
            outputing      = true;

          } else if (app->KeyboardKey(px::KeyboardKey::KEY_LEFT_SHIFT).held && px:: instanceof
                     <InputGate>(gate.get())) {
            SetInput(gate, (gate->state == State::OFF || gate->state == State::ERROR) ? State::ON : State::OFF);

          } else {
            selected_gate = gate;

            dragging_offset = app->MousePosWorld() - gate->pos;
            dragging_gate   = gate;
            dragging        = true;
          }
        } else if (app->MouseButton(px::MouseButton::BUTTON_1).released && outputing) {
          if (gate != outputing_gate && !px:: instanceof <InputGate>(gate.get())) {
            ToggleConnection(outputing_gate, gate);
          }

          outputing = false;
          outputing_gate.reset();
        }

        hovered_gate = gate;

      } else if (app->MouseButton(px::MouseButton::BUTTON_1).pressed && (selected_gate == gate)) {
        selected_gate.reset();
      }
    }

    if (app->KeyboardKey(px::KeyboardKey::KEY_DELETE).pressed && selected_gate) {
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

    if (app->MouseButton(px::MouseButton::BUTTON_1).pressed && adding) {
      adding = false;
      adding_gate.reset();

    } else if (app->MouseButton(px::MouseButton::BUTTON_1).released) {
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

    if (selected_gate) {
      px::Renderer::OutlineQuad(selected_gate->pos, selected_gate->size, {1.f, 1.f, 0.f, 1.f});

      for (auto&& input : selected_gate->inputs) {
        px::Renderer::OutlineQuad(input->pos, input->size, {0.f, 1.f, 1.f, 1.f});
        px::Renderer::DrawLine(input->pos + (input->size * glm::vec2(1.f, 0.5f)),
                               selected_gate->pos + (selected_gate->size * glm::vec2(0.f, 0.5f)),
                               0.1f,
                               {0.f, 1.f, 1.f, 1.f});
      }

      for (auto&& output : selected_gate->outputs) {
        px::Renderer::OutlineQuad(output->pos, output->size, {0.f, 1.f, 1.f, 1.f});
        px::Renderer::DrawLine(selected_gate->pos + (selected_gate->size * glm::vec2(1.f, 0.5f)),
                               output->pos + (output->size * glm::vec2(0.f, 0.5f)),
                               0.1f,
                               {0.f, 1.f, 1.f, 1.f});
      }
    }

    if (outputing) {
      px::Renderer::DrawLine(outputing_gate->pos + (outputing_gate->size * glm::vec2 {1.f, 0.5f}),
                             app->MousePosWorld());
      px::Renderer::OutlineQuad(outputing_gate->pos, outputing_gate->size, {1.f, 0.f, 1.f, 1.f});
      if (hovered_gate) px::Renderer::OutlineQuad(hovered_gate->pos, hovered_gate->size, {1.f, 0.f, 1.f, 1.f});
    }
  }

  void LogicCircuit::SetInput(const std::shared_ptr<LogicGate>& input, State val) {
    input->state = val;
    gates_update_pending.insert(input);
    gates_update_forced.insert(input);
  }

  void LogicCircuit::ToggleConnection(const std::shared_ptr<LogicGate>& from, const std::shared_ptr<LogicGate>& to) {
    if (from->outputs.contains(to) || to->inputs.contains(from)) {
      from->outputs.erase(to);
      to->inputs.erase(from);

    } else {
      from->outputs.insert(to);
      to->inputs.insert(from);
    }

    gates_update_pending.insert(to);
  }

  void LogicCircuit::MakeConnection(const px::UUID& from, const px::UUID& to) {
    std::shared_ptr<LogicGate> from_gate = *std::find_if(
        gates.begin(), gates.end(), [&from](const std::shared_ptr<LogicGate>& gate) { return gate->id == from; });

    std::shared_ptr<LogicGate> to_gate = *std::find_if(
        gates.begin(), gates.end(), [&to](const std::shared_ptr<LogicGate>& gate) { return gate->id == to; });

    from_gate->outputs.insert(to_gate);
    to_gate->inputs.insert(from_gate);

    gates_update_pending.insert(to_gate);
  }

  LogicCircuit::TruthTable LogicCircuit::ComputeTruthTable() {
    const size_t   num_inputs  = gates_input.size();
    const size_t   num_outputs = gates_output.size();
    const uint64_t num_entries = std::pow(2, num_inputs);

    std::unordered_map<px::UUID, State> previous_states;

    for (auto&& input : gates_input) {
      previous_states[input->id] = input->state;
    }

    LogicCircuit::TruthTable table;
    table.reserve(num_entries);

    for (uint64_t n = 0; n < num_entries; n++) {
      uint64_t current_gate = 0;

      TruthTableEntry current_entry = std::make_pair(std::vector<bool>(), std::vector<bool>());

      current_entry.first.reserve(num_inputs);
      current_entry.second.reserve(num_outputs);

      for (auto&& gate : gates_input) {
        const bool current_state = ((n >> current_gate++) & 1ULL) == true;

        SetInput(gate, current_state ? State::ON : State::OFF);
        current_entry.first.push_back(current_state);
      }

      UpdateLogicState();

      for (auto&& gate : gates_output) {
        current_entry.second.emplace_back(gate->state == State::ON ? true : false);
      }

      table.push_back(std::move(current_entry));
    }

    for (auto&& input : gates_input) {
      SetInput(input, previous_states[input->id]);
    }

    return table;
  }
}
