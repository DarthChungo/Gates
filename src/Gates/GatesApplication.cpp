#include "Engine/Renderer.hpp"
#include "Gates/GatesApplication.hpp"
#include "Gates/LogicGates.hpp"

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

    circuit.UpdateState();

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

        static char filename[120] = {};

        if (ImGui::BeginMenu("Guardar como...")) {
          ImGui::InputText(".gates", filename, 120);
          ImGui::SameLine();

          if (ImGui::Button("Guardar")) {
          }

          ImGui::EndMenu();
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

      if (ImGui::CollapsingHeader("Renderizado:")) {
        ImGui::Indent();
        ImGui::Text("Cuadrados: %d", Renderer::GetStats().quads_drawn);
        ImGui::Text("Triángulos: %d", Renderer::GetStats().tris_drawn);
        ImGui::Text("Círculos: %d", Renderer::GetStats().circles_drawn);
        ImGui::Text("Líneas: %d", Renderer::GetStats().lines_drawn);
        ImGui::Text("Cuadrados (resaltados): %d", Renderer::GetStats().quads_outlined);
        ImGui::Text("Presentaciones: %d", Renderer::GetStats().draw_calls);
        ImGui::Text("Vértices (triángulos): %d", Renderer::GetStats().tri_vertex_count);
        ImGui::Text("Índices (triángulos): %d", Renderer::GetStats().tri_index_count);
        ImGui::Text("Vértices (líneas): %d", Renderer::GetStats().line_vertex_count);
        ImGui::Text("Índices (líneas): %d", Renderer::GetStats().line_index_count);
        ImGui::Unindent();
      }

      if (ImGui::CollapsingHeader("Ratón:")) {
        ImGui::Indent();
        ImGui::TextWrapped("Posición (puntero): %s", std::to_string(MousePos()).c_str());
        ImGui::TextWrapped("Posición (rueda): %s", std::to_string(MouseWheel()).c_str());
        ImGui::TextWrapped("¿En la ventada? %s", MouseFocus() ? "yes" : "no");
        ImGui::Unindent();
      }

      if (ImGui::CollapsingHeader("Ventana:")) {
        ImGui::Indent();
        ImGui::TextWrapped("Tamaño: %s", std::to_string(WindowSize()).c_str());
        ImGui::TextWrapped("Posición: %s", std::to_string(WindowPos()).c_str());
        ImGui::Unindent();
      }

      if (ImGui::CollapsingHeader("Aplicación:")) {
        ImGui::Indent();
        ImGui::Text("FPS: %d f/s", fps());
        ImGui::Text("MSPF: %.4f ms/f", 1000 / (float)fps());
        ImGui::Unindent();
      }

      if (ImGui::CollapsingHeader("Cámara:")) {
        ImGui::Indent();
        ImGui::Text("Posición: %s", std::to_string(glm::vec2 {camera.getPosition().x, camera.getPosition().y}).c_str());
        ImGui::Text("Distancia de visión: %f", view_distance);
        ImGui::Unindent();
      }

      ImGui::End();
    }

    if (show_controls) {
      ImGui::Begin("Controles");

      if (ImGui::CollapsingHeader("Añadir")) {
        ImGui::Indent();

        if (ImGui::Button("Puerta input")) circuit.AddGate<InputGate>();
        if (ImGui::Button("Puerta output")) circuit.AddGate<OutputGate>();
        if (ImGui::Button("Puerta not")) circuit.AddGate<NotGate>();
        if (ImGui::Button("Puerta or")) circuit.AddGate<OrGate>();
        if (ImGui::Button("Puerta and")) circuit.AddGate<AndGate>();
        if (ImGui::Button("Puerta xor")) circuit.AddGate<XorGate>();

        ImGui::Unindent();
      }

      if (ImGui::CollapsingHeader("Puertas:")) {
        ImGui::Indent();

        for (auto&& gate : circuit.gates) {
          ImGui::Text("Puerta %s", gate->getName());

          ImGui::Indent();

          ImGui::Text("ID: %lu", (uint64_t)gate->id);
          ImGui::Text("Estado: %s", StateNames[gate->state]);

          ImGui::Text("Ins:");
          ImGui::Indent();

          for (auto&& in : gate->inputs) {
            ImGui::Text("ID: %lu", (uint64_t)in->id);
          }

          ImGui::Unindent();

          ImGui::Text("Outs:");
          ImGui::Indent();

          for (auto&& out : gate->outputs) {
            ImGui::Text("ID: %lu", (uint64_t)out->id);
          }

          ImGui::Unindent();
          ImGui::Unindent();
        }

        ImGui::Unindent();
      }

      ImGui::End();
    }

    Renderer::ResetStats();

    return rcode::ok;
  }

  rcode GatesApplication::pOnLaunch() {
    Renderer::Init();

    auto in1   = circuit.AddGateDirect<InputGate>({0.f, 0.f});
    auto gate1 = circuit.AddGateDirect<NotGate>({15.f, 0.f});
    auto out1  = circuit.AddGateDirect<OutputGate>({30.f, 0.f});

    circuit.ToggleConnection(in1, gate1);
    circuit.ToggleConnection(gate1, out1);

    return rcode::ok;
  }

  rcode GatesApplication::pOnRender() {
    Renderer::UseCamera(camera);
    Renderer::BeginBatch();

    circuit.DrawGates(MouseState());

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
