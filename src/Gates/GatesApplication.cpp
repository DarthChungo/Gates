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
#include "Gates/LogicGates.hpp"
#include "Gates/DataSerializer.hpp"

namespace Gates {
  px::rcode GatesApplication::pOnUpdate() {
    if (MouseButton(px::MouseButton::BUTTON_1).pressed && KeyboardKey(px::KeyboardKey::KEY_LEFT_CONTROL).held) {
      dragging = true;
      offset   = MousePosWorld();
    }

    if (MouseButton(px::MouseButton::BUTTON_1).released) dragging = false;

    if (dragging) {
      camera.offsetPosition(glm::vec3 {offset - MousePosWorld(), 0.f});
      offset = MousePosWorld();
    }

    view_distance = std::clamp(view_distance - (MouseWheel().y / 0.2f), .2f, 500.f);
    ResetMouseWheel();

    camera.setProjection(view_distance, AspectRatio());
    camera.setRotation(view_rotation);

    circuit.UpdateGraphicsState();
    circuit.UpdateLogicState();

    return px::rcode::ok;
  }

  px::rcode GatesApplication::pOnImGuiRender() {
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    const ImGuiIO&       io       = ImGui::GetIO();

    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    static const ImGuiWindowFlags win_flags =
        ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_MenuBar;

    ImGui::Begin("main_dockspace_window", nullptr, win_flags);

    ImGui::PopStyleVar(3);

    static const ImGuiDockNodeFlags ds_flags = ImGuiDockNodeFlags_PassthruCentralNode;

    ImGuiID dockspace_id = ImGui::GetID("main_dockspace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

    if (static bool first_time = true; first_time && !(first_time = false)) {
      ImGui::DockBuilderRemoveNode(dockspace_id);
      ImGui::DockBuilderAddNode(dockspace_id, ds_flags | ImGuiDockNodeFlags_DockSpace);
      ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

      ImGui::DockBuilderDockWindow(
          "Tabla de verdad", ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, .2f, nullptr, &dockspace_id));

      ImGui::DockBuilderDockWindow(
          "Controles", ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, .2f, nullptr, &dockspace_id));

      ImGui::DockBuilderFinish(dockspace_id);
    }

    ImGui::End();

    static bool open_version_modal = false;
    static bool open_license_modal = false;

    if (ImGui::BeginMainMenuBar()) {
      if (ImGui::BeginMenu("Archivo")) {
        if (ImGui::MenuItem("Salir sin guardar")) Close();

        static ReadStatus read_status = ReadStatus::UNSET;

        if (ImGui::BeginMenu("Abrir circuito...")) {
          const auto& list = DataSerializer::ListCircuitFiles();

          if (list.empty()) {
            ImGui::TextDisabled("No se ha guardado ningún circuito");
          }

          if (read_status == ReadStatus::UNSET) {
            for (const std::string& name : DataSerializer::ListCircuitFiles()) {
              if (ImGui::Button(name.c_str())) {
                read_status = DataSerializer::ParseCircuitFile(name.c_str(), circuit);
              }
            }

          } else {
            ImGui::TextColored(is_error(read_status) ? ImVec4(1.f, 0.f, 0.f, 1.f) : ImVec4(0.f, 1.f, 0.f, 1.f),
                               "%s",
                               ReadStatusReadableName[read_status]);
          }

          ImGui::EndMenu();

        } else {
          read_status = ReadStatus::UNSET;
        }

        static char        filename[256] = {};
        static WriteStatus write_status  = WriteStatus::UNSET;

        if (ImGui::BeginMenu("Guardar como...")) {
          ImGui::InputText("##save_input", filename, 256);

          ImGui::SameLine();
          ImGui::TextDisabled(".gates");
          ImGui::SameLine();

          if (ImGui::Button("Guardar")) {
            write_status = DataSerializer::WriteCircuitFile(filename, circuit, false);
          }

          if (write_status == WriteStatus::ERR_FILE_EXISTS) {
            ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "%s", WriteStatusReadableName[WriteStatus::ERR_FILE_EXISTS]);

            ImGui::SameLine();
            if (ImGui::Button("Sobreescribir")) {
              write_status = DataSerializer::WriteCircuitFile(filename, circuit, true);
            }
          }

          if (write_status != WriteStatus::UNSET && write_status != WriteStatus::ERR_FILE_EXISTS) {
            ImGui::TextColored(is_error(write_status) ? ImVec4(1.f, 0.f, 0.f, 1.f) : ImVec4(0.f, 1.f, 0.f, 1.f),
                               "%s",
                               WriteStatusReadableName[write_status]);
          }

          ImGui::EndMenu();

        } else {
          *filename    = 0;
          write_status = WriteStatus::UNSET;
        }

        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("Ver")) {
        ImGui::Checkbox("Tabla de verdad", &show_truthtable);
        ImGui::Checkbox("Controles", &show_controls);
        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("Acerca de")) {
        if (ImGui::MenuItem("Licencia")) {
          open_license_modal = true;
        }

        if (ImGui::MenuItem("Versión")) {
          open_version_modal = true;
        }

        ImGui::EndMenu();
      }

      ImGui::EndMainMenuBar();
    }

    if (open_version_modal) {
      ImGui::OpenPopup("Versión");

      ImGui::SetNextWindowPos(viewport->GetCenter(), ImGuiCond_Appearing, {0.5f, 0.5f});

      if (ImGui::BeginPopupModal("Versión", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {
        ImGui::TextUnformatted("Gates " GATES_APPLICATION_VERSION_TXT);
        ImGui::TextUnformatted("Programado por DarthChungo (https://github.com/DarthChungo)");
        ImGui::TextUnformatted("Código disponible en https://github.com/DarthChungo/Gates");

        if (ImGui::Button("Cerrar")) {
          ImGui::CloseCurrentPopup();
          open_version_modal = false;
        }

        ImGui::EndPopup();
      }

    } else if (open_license_modal) {
      ImGui::OpenPopup("Licencia");

      ImGui::SetNextWindowPos(viewport->GetCenter(), ImGuiCond_Appearing, {0.5f, 0.5f});

      if (ImGui::BeginPopupModal("Licencia", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {
        ImGui::TextUnformatted(GATES_APPLICATION_LICENSE);

        if (ImGui::Button("Cerrar")) {
          ImGui::CloseCurrentPopup();
          open_license_modal = false;
        }

        ImGui::EndPopup();
      }
    }

    static const ImVec4 less_attention_color(0.65f, 0.65f, 0.65f, 1.f);
    static const ImVec4 normal_attention_color(1.f, 1.f, 1.f, 1.f);

    static float truthtable_window_width = 0;

    if (show_truthtable) {
      if (ImGui::Begin("Tabla de verdad", nullptr, ImGuiWindowFlags_HorizontalScrollbar)) {
        truthtable_window_width = ImGui::GetWindowWidth();

        if (ImGui::TreeNodeEx(
                "Tabla de verdad",
                ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen)) {
          static LogicCircuit::TruthTable table;

          const bool disable_calc = circuit.gates.size() == 0;

          if (disable_calc) ImGui::BeginDisabled();

          if (ImGui::Button("Calcular...")) {
            table = circuit.ComputeTruthTable();
          }

          if (disable_calc) ImGui::EndDisabled();

          if (table.size() != 0 && table[0].first.size() != 0) {
            static uint32_t columns_ins  = table[0].first.size();
            static uint32_t columns_outs = table[0].second.size();

            if (ImGui::BeginTable(
                    "##ttable1", columns_ins + columns_outs, ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerV)) {
              for (uint32_t i = 0; i < columns_ins; i++) ImGui::TableSetupColumn(("in" + std::to_string(i)).c_str());
              for (uint32_t i = 0; i < columns_outs; i++) ImGui::TableSetupColumn(("out" + std::to_string(i)).c_str());

              ImGui::TableHeadersRow();

              for (const auto& entry : table) {
                ImGui::TableNextRow();
                uint32_t colum = 0;

                for (const auto& in : entry.first) {
                  ImGui::TableSetColumnIndex(colum);
                  ImGui::TextColored(in ? normal_attention_color : less_attention_color, "%s", in ? "1" : "0");
                  colum++;
                }

                for (const auto& out : entry.second) {
                  ImGui::TableSetColumnIndex(colum);
                  ImGui::TextColored(out ? normal_attention_color : less_attention_color, "%s", out ? "1" : "0");
                  colum++;
                }
              }

              ImGui::EndTable();
            }
          }

          ImGui::TreePop();
        }

        ImGui::End();
      }
    }

    if (show_controls) {
      ImGui::Begin("Controles", nullptr, ImGuiWindowFlags_HorizontalScrollbar);

      if (ImGui::TreeNodeEx(
              "Añadir",
              ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen)) {
        if (ImGui::Button("Puerta input")) circuit.AddGate<InputGate>();
        if (ImGui::Button("Puerta output")) circuit.AddGate<OutputGate>();
        if (ImGui::Button("Puerta not")) circuit.AddGate<NotGate>();
        if (ImGui::Button("Puerta or")) circuit.AddGate<OrGate>();
        if (ImGui::Button("Puerta and")) circuit.AddGate<AndGate>();
        if (ImGui::Button("Puerta xor")) circuit.AddGate<XorGate>();
        ImGui::TreePop();
      }

      if (ImGui::TreeNodeEx(
              "Puertas:",
              ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen)) {
        const auto DisplayGate = [&](const auto& self, const std::shared_ptr<LogicGate>& gate) -> void {
          ImGui::PushID(gate->id);

          if (ImGui::TreeNodeEx((std::string("Puerta ") + gate->getName()).c_str(), ImGuiTreeNodeFlags_SpanFullWidth)) {
            ImGui::PushStyleColor(ImGuiCol_Text, less_attention_color);

            ImGui::TreeNodeEx("##gstate",
                              ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet |
                                  ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth,
                              "Estado %s",
                              StateReadableNames[gate->state]);

            ImGui::TreeNodeEx("##gstate",
                              ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet |
                                  ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth,
                              "ID 0x%lx",
                              (uint64_t)gate->id);

            if (ImGui::TreeNodeEx("Salidas", ImGuiTreeNodeFlags_SpanFullWidth)) {
              ImGui::PopStyleColor();

              for (const std::shared_ptr<LogicGate>& output : gate->outputs) {
                self(self, output);
              }

              ImGui::TreePop();

            } else {
              ImGui::PopStyleColor();
            }

            ImGui::PushStyleColor(ImGuiCol_Text, less_attention_color);

            if (ImGui::TreeNodeEx("Entradas", ImGuiTreeNodeFlags_SpanFullWidth)) {
              ImGui::PopStyleColor();

              for (const std::shared_ptr<LogicGate>& input : gate->inputs) {
                self(self, input);
              }

              ImGui::TreePop();

            } else {
              ImGui::PopStyleColor();
            }

            ImGui::TreePop();
          }

          ImGui::PopID();
        };

        for (const std::shared_ptr<LogicGate>& gate : circuit.gates) {
          DisplayGate(DisplayGate, gate);
        }

        ImGui::TreePop();
      }

      ImGui::End();
    }

    const ImGuiWindowFlags overlay_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
                                           ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                                           ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav |
                                           ImGuiWindowFlags_NoMove;

    ImGui::SetNextWindowPos(
        viewport->WorkSize + viewport->WorkPos - ImVec2(10.f, 10.f) - ImVec2(truthtable_window_width, 0.f),
        ImGuiCond_Always,
        {1.f, 1.f});
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::SetNextWindowBgAlpha(0.35f);

    if (ImGui::Begin("selection_overlay", nullptr, overlay_flags)) {
      if (const std::shared_ptr<LogicGate>& gate = circuit.selected_gate; circuit.selected_gate) {
        ImGui::Text("Selección: \n - Tipo %s\n - Estado %s\n - Entradas %lu\n - Salidas %lu",
                    gate->getName(),
                    StateReadableNames[gate->state],
                    gate->inputs.size(),
                    gate->outputs.size());

      } else {
        ImGui::TextDisabled("Ninguna puerta seleccionada");
      }
    }

    ImGui::End();

    px::Renderer::ResetStats();

    return px::rcode::ok;
  }

  px::rcode GatesApplication::pOnLaunch() {
    px::Renderer::Init();
    return px::rcode::ok;
  }

  px::rcode GatesApplication::pOnRender() {
    px::Renderer::UseCamera(camera);
    px::Renderer::BeginBatch();

    circuit.DrawGates();

    px::Renderer::EndBatch();
    px::Renderer::FlushBatch();

    return px::rcode::ok;
  }

  px::rcode GatesApplication::pOnClose() {
    px::Renderer::Delete();
    return px::rcode::ok;
  }

  glm::vec2 GatesApplication::MousePosWorld() { return camera.ScreenToWorld(MousePos(), view_distance, AspectRatio()); }
}
