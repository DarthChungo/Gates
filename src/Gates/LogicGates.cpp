#include "Gates/LogicGates.hpp"
#include "Engine/Renderer.hpp"

namespace Gates {
  NotGate::NotGate() : LogicGate() { size = glm::vec2 {8.f, 10.f}; }
  NotGate::NotGate(const UUID& uuid) : LogicGate(uuid) { size = glm::vec2 {8.f, 10.f}; };

  void NotGate::UpdateState() {
    if (inputs.size() != 1 || inputs.begin()->get()->state == State::ERROR) {
      state = State::ERROR;

    } else if (inputs.begin()->get()->state == State::OFF) {
      state = State::ON;

    } else {
      state = State::OFF;
    }
  }

  void NotGate::Draw() {
    Renderer::BorderTri(pos, pos + glm::vec2(5.3f, 5.f), pos + glm::vec2(0.f, 10.f), 0.3f, getColor());
    Renderer::BorderCircle(pos + glm::vec2(6.5f, 5.f), 1.5f, 20, 0.3f, getColor());

    for (auto&& output : outputs) {
      Renderer::DrawLine(pos + glm::vec2(8.f, 5.f), output->pos + glm::vec2(0.f, 5.f), 0.1f, getColor());
    }
  }

  AndGate::AndGate() : LogicGate() { size = glm::vec2 {12.5, 10.f}; }
  AndGate::AndGate(const UUID& uuid) : LogicGate(uuid) { size = glm::vec2 {12.5, 10.f}; }

  void AndGate::UpdateState() {
    if (inputs.size() < 2) {
      state = State::ERROR;

    } else {
      uint32_t count = 0;

      for (auto&& input : inputs) {
        switch (input->state) {
          case State::ERROR:
            state = State::ERROR;
            return;

          case State::ON:
            count++;

          case State::OFF:
            state = State::ON;
            break;
        }
      }

      state = count == inputs.size() ? State::ON : State::OFF;
    }
  }

  void AndGate::Draw() {
    Renderer::DrawQuad(pos, glm::vec2(0.3f, 10.f));
    Renderer::DrawQuad(pos, glm::vec2(7.5f, 0.3f));
    Renderer::DrawQuad(pos + glm::vec2(0.f, 9.7f), glm::vec2(7.5f, 0.3f));
    Renderer::DrawQuad(pos + glm::vec2(0.3f, 0.3f), glm::vec2(7.2f, 9.4f), getColor());

    Renderer::BorderSemicircle(pos + glm::vec2(7.5f, 5.f), 5.f, -1.570f, 1.570f, 30, 0.3f, getColor());

    for (auto&& output : outputs) {
      Renderer::DrawLine(pos + glm::vec2(12.5f, 5.f), output->pos + glm::vec2(0.f, 5.f), 0.1f, getColor());
    }
  }

  OrGate::OrGate() : LogicGate() { size = glm::vec2 {12.5f, 10.f}; }
  OrGate::OrGate(const UUID& uuid) : LogicGate(uuid) { size = glm::vec2 {12.5f, 10.f}; }

  void OrGate::UpdateState() {
    if (inputs.size() < 2) {
      state = State::ERROR;

    } else {
      for (auto&& input : inputs) {
        switch (input->state) {
          case State::ERROR:
            state = State::ERROR;
            return;

          case State::ON:
            state = State::ON;
            return;

          case State::OFF:
            state = State::OFF;
            break;
        }
      }
    }
  }

  void OrGate::Draw() {
    Renderer::BorderSemicircleCustomCenterOutside(
        pos + glm::vec2(-3.75f, 5.f), pos + glm::vec2(7.f, 5.f), 6.25f, -0.927f, 0.927f, 20, 0.3f, getColor());

    Renderer::BorderSemicircleCustomCenterInside(pos + glm::vec2(5.f, 1.875f),
                                                 pos + glm::vec2(5.f, 5.f),
                                                 8.125f,
                                                 0.394f,
                                                 glm::pi<float>() / 2.f,
                                                 30,
                                                 0.3f,
                                                 getColor());

    Renderer::BorderSemicircleCustomCenterInside(pos + glm::vec2(5.f, 8.125f),
                                                 pos + glm::vec2(5.f, 5.f),
                                                 8.125f,
                                                 -glm::pi<float>() / 2.f,
                                                 -0.394f,
                                                 30,
                                                 0.3f,
                                                 getColor());

    Renderer::BorderSemicircleCustomCenterInside(pos + glm::vec2(2.5f, 7.5f),
                                                 pos + glm::vec2(5.f, 5.f),
                                                 3.535f,
                                                 glm::pi<float>() / 4.f,
                                                 3 * glm::pi<float>() / 4.f,
                                                 1,
                                                 0.3f,
                                                 getColor());

    Renderer::DrawTri(pos + glm::vec2(5.f, 5.f), pos + glm::vec2(5.f, 9.7f), pos + glm::vec2(4.7f, 9.7f), getColor());
    Renderer::DrawTri(pos + glm::vec2(5.f, 10.f), pos + glm::vec2(5.f, 9.7f), pos + glm::vec2(4.7f, 9.7f));

    Renderer::BorderSemicircleCustomCenterInside(pos + glm::vec2(2.5f, 2.5f),
                                                 pos + glm::vec2(5.f, 5.f),
                                                 3.535f,
                                                 -3.f * glm::pi<float>() / 4.f,
                                                 -glm::pi<float>() / 4.f,
                                                 1,
                                                 0.3f,
                                                 getColor());

    Renderer::DrawTri(pos + glm::vec2(5.f, 5.f), pos + glm::vec2(5.f, 0.3f), pos + glm::vec2(4.7f, 0.3f), getColor());
    Renderer::DrawTri(pos + glm::vec2(5.f, 0.f), pos + glm::vec2(5.f, 0.3f), pos + glm::vec2(4.7f, 0.3f));

    for (auto&& output : outputs) {
      Renderer::DrawLine(
          pos + (size * glm::vec2 {1.f, 0.5f}), output->pos + (size * glm::vec2 {0.f, 0.5f}), 0.1f, getColor());
    }
  }

  XorGate::XorGate() : LogicGate() { size = glm::vec2 {12.5f, 10.f}; }
  XorGate::XorGate(const UUID& uuid) : LogicGate(uuid) { size = glm::vec2 {12.5f, 10.f}; }

  void XorGate::UpdateState() {
    if (inputs.size() < 2) {
      state = State::ERROR;

    } else {
      uint32_t count = 0;

      for (auto&& input : inputs) {
        switch (input->state) {
          case State::ERROR:
            state = State::ERROR;
            return;

          case State::ON:
            count++;

          case State::OFF:
            break;
        }
      }

      state = count == 1 ? State::ON : State::OFF;
    }
  }

  void XorGate::Draw() {
    Renderer::BorderSemicircleCustomCenterOutside(
        pos + glm::vec2(-3.75f, 5.f), pos + glm::vec2(7.f, 5.f), 6.25f, -0.927f, 0.927f, 20, 0.3f, getColor());

    Renderer::BorderSemicircleCustomCenterOutside(
        pos + glm::vec2(-3.75, 5.f), pos + glm::vec2(7.f, 5.f), 5.25f, -1.2f, 1.2f, 20, 0.3f, {0.f, 0.f, 0.f, 0.f});

    Renderer::BorderSemicircleCustomCenterInside(pos + glm::vec2(5.f, 1.875f),
                                                 pos + glm::vec2(5.f, 5.f),
                                                 8.125f,
                                                 0.394f,
                                                 glm::pi<float>() / 2.f,
                                                 30,
                                                 0.3f,
                                                 getColor());

    Renderer::BorderSemicircleCustomCenterInside(pos + glm::vec2(5.f, 8.125f),
                                                 pos + glm::vec2(5.f, 5.f),
                                                 8.125f,
                                                 -glm::pi<float>() / 2.f,
                                                 -0.394f,
                                                 30,
                                                 0.3f,
                                                 getColor());

    Renderer::BorderSemicircleCustomCenterInside(pos + glm::vec2(2.5f, 7.5f),
                                                 pos + glm::vec2(5.f, 5.f),
                                                 3.535f,
                                                 glm::pi<float>() / 4.f,
                                                 3 * glm::pi<float>() / 4.f,
                                                 1,
                                                 0.3f,
                                                 getColor());

    Renderer::DrawTri(pos + glm::vec2(5.f, 5.f), pos + glm::vec2(5.f, 9.7f), pos + glm::vec2(4.7f, 9.7f), getColor());
    Renderer::DrawTri(pos + glm::vec2(5.f, 10.f), pos + glm::vec2(5.f, 9.7f), pos + glm::vec2(4.7f, 9.7f));

    Renderer::BorderSemicircleCustomCenterInside(pos + glm::vec2(2.5f, 2.5f),
                                                 pos + glm::vec2(5.f, 5.f),
                                                 3.535f,
                                                 -3.f * glm::pi<float>() / 4.f,
                                                 -glm::pi<float>() / 4.f,
                                                 1,
                                                 0.3f,
                                                 getColor());

    Renderer::DrawTri(pos + glm::vec2(5.f, 5.f), pos + glm::vec2(5.f, 0.3f), pos + glm::vec2(4.7f, 0.3f), getColor());
    Renderer::DrawTri(pos + glm::vec2(5.f, 0.f), pos + glm::vec2(5.f, 0.3f), pos + glm::vec2(4.7f, 0.3f));

    for (auto&& output : outputs) {
      Renderer::DrawLine(
          pos + (size * glm::vec2 {1.f, 0.5f}), output->pos + (size * glm::vec2 {0.f, 0.5f}), 0.1f, getColor());
    }
  }

  InputGate::InputGate() : LogicGate() { size = glm::vec2 {10.f, 10.f}; }
  InputGate::InputGate(const UUID& uuid) : LogicGate(uuid) { size = glm::vec2 {10.f, 10.f}; }

  void InputGate::UpdateState() {}

  void InputGate::Draw() {
    Renderer::BorderCircle(pos + glm::vec2(5.f, 5.f), 5.f, 30, 0.3f, getColor());

    for (auto&& output : outputs) {
      Renderer::DrawLine(pos + glm::vec2(10.f, 5.f), output->pos + glm::vec2(0.f, 5.f), 0.1f, getColor());
    }
  }

  OutputGate::OutputGate() : LogicGate() { size = glm::vec2 {10.f, 10.f}; }
  OutputGate::OutputGate(const UUID& uuid) : LogicGate(uuid) { size = glm::vec2 {10.f, 10.f}; }

  void OutputGate::UpdateState() {
    if (inputs.size() != 1) {
      state = State::ERROR;
    } else {
      state = inputs.begin()->get()->state;
    }
  }

  void OutputGate::Draw() { Renderer::BorderCircle(pos + glm::vec2(5.f, 5.f), 5.f, 30, 0.3f, getColor()); }
}
