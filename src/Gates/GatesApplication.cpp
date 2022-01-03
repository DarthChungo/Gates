#include "Engine/Renderer.hpp"
#include "Gates/GatesApplication.hpp"
#include "Gates/LogicGates.hpp"
#include "Gates/DataSerializer.hpp"
#include "Util/Logger.hpp"

namespace Gates {
  rcode GatesApplication::pOnUpdate() {
    if (KeyboardKey(KeyboardKey::KEY_CAPS_LOCK).pressed) Close();

    if (MouseButton(MouseButton::BUTTON_1).pressed && KeyboardKey(KeyboardKey::KEY_LEFT_CONTROL).held) {
      dragging = true;
      offset   = MousePosWorld();
    }

    if (MouseButton(MouseButton::BUTTON_1).released) dragging = false;

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

    return rcode::ok;
  }

  rcode GatesApplication::pOnImGuiRender() {
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
                                    ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                    ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
                                    ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_MenuBar;

    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace", nullptr, window_flags);
    ImGui::PopStyleVar();
    ImGui::PopStyleVar(2);

    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

    static bool first_time = true;

    if (first_time && !(first_time = false)) {
      ImGui::DockBuilderRemoveNode(dockspace_id);
      ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
      ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

      ImGui::DockBuilderDockWindow(
          "Estadísticas", ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, .25f, nullptr, &dockspace_id));

      ImGui::DockBuilderDockWindow(
          "Controles", ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, .4f, nullptr, &dockspace_id));

      ImGui::DockBuilderFinish(dockspace_id);
    }

    ImGui::End();

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
        ImGui::Checkbox("Estadísticas", &show_statistics);
        ImGui::Checkbox("Controles", &show_controls);
        ImGui::EndMenu();
      }

      ImGui::EndMainMenuBar();
    }

    if (show_statistics) {
      ImGui::Begin("Estadísticas");

      if (ImGui::TreeNodeEx("Renderizado:", ImGuiTreeNodeFlags_SpanFullWidth)) {
        ImGui::Text("Cuadrados: %d", Renderer::GetStats().quads_drawn);
        ImGui::Text("Triángulos: %d", Renderer::GetStats().tris_drawn);
        ImGui::Text("Triángulos (resaltados): %d", Renderer::GetStats().tris_outlined);
        ImGui::Text("Triángulos (bordeados): %d", Renderer::GetStats().tris_bordered);
        ImGui::Text("Círculos: %d", Renderer::GetStats().circles_drawn);
        ImGui::Text("Círculos (resaltados): %d", Renderer::GetStats().circles_outlined);
        ImGui::Text("Círculos (bordeados): %d", Renderer::GetStats().circles_bordered);
        ImGui::Text("Semicírculos (bordeados): %d", Renderer::GetStats().semicircles_bordered);
        ImGui::Text("Líneas: %d", Renderer::GetStats().lines_drawn);
        ImGui::Text("Líneas (con grosor): %d", Renderer::GetStats().wide_lines_drawn);
        ImGui::Text("Cuadrados (resaltados): %d", Renderer::GetStats().quads_outlined);
        ImGui::Text("Presentaciones: %d", Renderer::GetStats().draw_calls);
        ImGui::Text("Vértices (triángulos): %d", Renderer::GetStats().tri_vertex_count);
        ImGui::Text("Índices (triángulos): %d", Renderer::GetStats().tri_index_count);
        ImGui::Text("Vértices (líneas): %d", Renderer::GetStats().line_vertex_count);
        ImGui::Text("Índices (líneas): %d", Renderer::GetStats().line_index_count);
        ImGui::TreePop();
      }

      if (ImGui::TreeNodeEx("Ratón:", ImGuiTreeNodeFlags_SpanFullWidth)) {
        ImGui::TextWrapped("Posición (puntero): %s", std::to_string(MousePos()).c_str());
        ImGui::TextWrapped("Posición (rueda): %s", std::to_string(MouseWheel()).c_str());
        ImGui::TextWrapped("¿En la ventada? %s", MouseFocus() ? "yes" : "no");
        ImGui::TreePop();
      }

      if (ImGui::TreeNodeEx("Ventana:", ImGuiTreeNodeFlags_SpanFullWidth)) {
        ImGui::TextWrapped("Tamaño: %s", std::to_string(WindowSize()).c_str());
        ImGui::TextWrapped("Posición: %s", std::to_string(WindowPos()).c_str());
        ImGui::TreePop();
      }

      if (ImGui::TreeNodeEx("Aplicación:", ImGuiTreeNodeFlags_SpanFullWidth)) {
        ImGui::Text("FPS: %d f/s", fps());
        ImGui::Text("MSPF: %.4f ms/f", 1000 / (float)fps());
        ImGui::TreePop();
      }

      if (ImGui::TreeNodeEx("Cámara:", ImGuiTreeNodeFlags_SpanFullWidth)) {
        ImGui::Text("Posición: %s", std::to_string(glm::vec2 {camera.getPosition().x, camera.getPosition().y}).c_str());
        ImGui::Text("Distancia de visión: %f", view_distance);
        ImGui::TreePop();
      }

      ImGui::End();
    }

    if (show_controls) {
      ImGui::Begin("Controles");

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
        static const ImVec4 less_attention_color(0.6f, 0.6f, 0.6f, 1.f);

        const auto DisplayGate = [&](const auto& self, const std::shared_ptr<LogicGate>& gate) -> void {
          ImGui::PushID(gate->id);

          if (ImGui::TreeNodeEx((std::string("Puerta ") + gate->getName()).c_str(), ImGuiTreeNodeFlags_SpanFullWidth)) {
            ImGui::PushStyleColor(ImGuiCol_Text, less_attention_color);

            ImGui::TreeNodeEx("##gstate",
                              ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet |
                                  ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth,
                              "Estado %s",
                              gate->state == State::OFF ? "off" : (gate->state == State::ON ? "on" : "error"));

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

      if (ImGui::TreeNodeEx(
              "Tabla de verdad",
              ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen)) {
        static LogicCircuit::TruthTable table;

        if (ImGui::Button("Calcular...")) {
          table = circuit.ComputeTruthTable();
        }

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
                ImGui::Text("%s", in ? "1" : "0");
                colum++;
              }

              for (const auto& out : entry.second) {
                ImGui::TableSetColumnIndex(colum);
                ImGui::Text("%s", out ? "1" : "0");
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

    Renderer::ResetStats();

    return rcode::ok;
  }

  rcode GatesApplication::pOnLaunch() {
    Renderer::Init();
    return rcode::ok;
  }

  rcode GatesApplication::pOnRender() {
    Renderer::UseCamera(camera);
    Renderer::BeginBatch();

    circuit.DrawGates();

    Renderer::EndBatch();
    Renderer::FlushBatch();

    return rcode::ok;
  }

  rcode GatesApplication::pOnClose() {
    Renderer::Delete();
    return rcode::ok;
  }

  glm::vec2 GatesApplication::MousePosWorld() { return camera.ScreenToWorld(MousePos(), view_distance, AspectRatio()); }
}
