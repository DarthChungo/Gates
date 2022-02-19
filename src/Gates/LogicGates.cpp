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

#include "Gates/LogicGates.hpp"

namespace Gates {
  NotGate::NotGate() : LogicGate() { size = glm::vec2 {8.f, 10.f}; }
  NotGate::NotGate(const px::UUID& uuid) : LogicGate(uuid) { size = glm::vec2 {8.f, 10.f}; };

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
    px::Renderer::BorderTri(pos, pos + glm::vec2(5.3f, 5.f), pos + glm::vec2(0.f, 10.f), 0.3f, getColor());
    px::Renderer::BorderCircle(pos + glm::vec2(6.5f, 5.f), 1.5f, 20, 0.3f, getColor());

    for (auto&& output : outputs) {
      px::Renderer::DrawLine(pos + glm::vec2(8.f, 5.f), output->pos + glm::vec2(0.f, 5.f), 0.1f, getColor());
    }
  }

  AndGate::AndGate() : LogicGate() { size = glm::vec2 {12.5, 10.f}; }
  AndGate::AndGate(const px::UUID& uuid) : LogicGate(uuid) { size = glm::vec2 {12.5, 10.f}; }

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
    px::Renderer::DrawQuad(pos, glm::vec2(0.3f, 10.f));
    px::Renderer::DrawQuad(pos, glm::vec2(7.5f, 0.3f));
    px::Renderer::DrawQuad(pos + glm::vec2(0.f, 9.7f), glm::vec2(7.5f, 0.3f));
    px::Renderer::DrawQuad(pos + glm::vec2(0.3f, 0.3f), glm::vec2(7.2f, 9.4f), getColor());

    px::Renderer::BorderSemicircle(pos + glm::vec2(7.5f, 5.f), 5.f, -1.570f, 1.570f, 30, 0.3f, getColor());

    for (auto&& output : outputs) {
      px::Renderer::DrawLine(pos + glm::vec2(12.5f, 5.f), output->pos + glm::vec2(0.f, 5.f), 0.1f, getColor());
    }
  }

  OrGate::OrGate() : LogicGate() { size = glm::vec2 {12.5f, 10.f}; }
  OrGate::OrGate(const px::UUID& uuid) : LogicGate(uuid) { size = glm::vec2 {12.5f, 10.f}; }

  void OrGate::UpdateState() {
    if (inputs.size() < 2) {
      state = State::ERROR;

    } else {
      state = State::OFF;

      for (auto&& input : inputs) {
        switch (input->state) {
          case State::ERROR:
            state = State::ERROR;
            return;

          case State::ON:
            state = State::ON;
            break;

          case State::OFF:
            break;
        }
      }
    }
  }

  void OrGate::Draw() {
    px::Renderer::BorderSemicircleCustomCenterOutside(
        pos + glm::vec2(-3.75f, 5.f), pos + glm::vec2(7.f, 5.f), 6.25f, -0.927f, 0.927f, 20, 0.3f, getColor());

    px::Renderer::BorderSemicircleCustomCenterInside(pos + glm::vec2(5.f, 1.875f),
                                                     pos + glm::vec2(5.f, 5.f),
                                                     8.125f,
                                                     0.394f,
                                                     glm::pi<float>() / 2.f,
                                                     30,
                                                     0.3f,
                                                     getColor());

    px::Renderer::BorderSemicircleCustomCenterInside(pos + glm::vec2(5.f, 8.125f),
                                                     pos + glm::vec2(5.f, 5.f),
                                                     8.125f,
                                                     -glm::pi<float>() / 2.f,
                                                     -0.394f,
                                                     30,
                                                     0.3f,
                                                     getColor());

    px::Renderer::BorderSemicircleCustomCenterInside(pos + glm::vec2(2.5f, 7.5f),
                                                     pos + glm::vec2(5.f, 5.f),
                                                     3.535f,
                                                     glm::pi<float>() / 4.f,
                                                     3 * glm::pi<float>() / 4.f,
                                                     1,
                                                     0.3f,
                                                     getColor());

    px::Renderer::DrawTri(
        pos + glm::vec2(5.f, 5.f), pos + glm::vec2(5.f, 9.7f), pos + glm::vec2(4.7f, 9.7f), getColor());
    px::Renderer::DrawTri(pos + glm::vec2(5.f, 10.f), pos + glm::vec2(5.f, 9.7f), pos + glm::vec2(4.7f, 9.7f));

    px::Renderer::BorderSemicircleCustomCenterInside(pos + glm::vec2(2.5f, 2.5f),
                                                     pos + glm::vec2(5.f, 5.f),
                                                     3.535f,
                                                     -3.f * glm::pi<float>() / 4.f,
                                                     -glm::pi<float>() / 4.f,
                                                     1,
                                                     0.3f,
                                                     getColor());

    px::Renderer::DrawTri(
        pos + glm::vec2(5.f, 5.f), pos + glm::vec2(5.f, 0.3f), pos + glm::vec2(4.7f, 0.3f), getColor());
    px::Renderer::DrawTri(pos + glm::vec2(5.f, 0.f), pos + glm::vec2(5.f, 0.3f), pos + glm::vec2(4.7f, 0.3f));

    for (auto&& output : outputs) {
      px::Renderer::DrawLine(
          pos + (size * glm::vec2 {1.f, 0.5f}), output->pos + (size * glm::vec2 {0.f, 0.5f}), 0.1f, getColor());
    }
  }

  XorGate::XorGate() : LogicGate() { size = glm::vec2 {12.5f, 10.f}; }
  XorGate::XorGate(const px::UUID& uuid) : LogicGate(uuid) { size = glm::vec2 {12.5f, 10.f}; }

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
    px::Renderer::BorderSemicircleCustomCenterOutside(
        pos + glm::vec2(-3.75f, 5.f), pos + glm::vec2(7.f, 5.f), 6.25f, -0.927f, 0.927f, 20, 0.3f, getColor());

    px::Renderer::BorderSemicircleCustomCenterOutside(
        pos + glm::vec2(-3.75, 5.f), pos + glm::vec2(7.f, 5.f), 5.25f, -1.2f, 1.2f, 20, 0.3f, {0.f, 0.f, 0.f, 0.f});

    px::Renderer::BorderSemicircleCustomCenterInside(pos + glm::vec2(5.f, 1.875f),
                                                     pos + glm::vec2(5.f, 5.f),
                                                     8.125f,
                                                     0.394f,
                                                     glm::pi<float>() / 2.f,
                                                     30,
                                                     0.3f,
                                                     getColor());

    px::Renderer::BorderSemicircleCustomCenterInside(pos + glm::vec2(5.f, 8.125f),
                                                     pos + glm::vec2(5.f, 5.f),
                                                     8.125f,
                                                     -glm::pi<float>() / 2.f,
                                                     -0.394f,
                                                     30,
                                                     0.3f,
                                                     getColor());

    px::Renderer::BorderSemicircleCustomCenterInside(pos + glm::vec2(2.5f, 7.5f),
                                                     pos + glm::vec2(5.f, 5.f),
                                                     3.535f,
                                                     glm::pi<float>() / 4.f,
                                                     3 * glm::pi<float>() / 4.f,
                                                     1,
                                                     0.3f,
                                                     getColor());

    px::Renderer::DrawTri(
        pos + glm::vec2(5.f, 5.f), pos + glm::vec2(5.f, 9.7f), pos + glm::vec2(4.7f, 9.7f), getColor());
    px::Renderer::DrawTri(pos + glm::vec2(5.f, 10.f), pos + glm::vec2(5.f, 9.7f), pos + glm::vec2(4.7f, 9.7f));

    px::Renderer::BorderSemicircleCustomCenterInside(pos + glm::vec2(2.5f, 2.5f),
                                                     pos + glm::vec2(5.f, 5.f),
                                                     3.535f,
                                                     -3.f * glm::pi<float>() / 4.f,
                                                     -glm::pi<float>() / 4.f,
                                                     1,
                                                     0.3f,
                                                     getColor());

    px::Renderer::DrawTri(
        pos + glm::vec2(5.f, 5.f), pos + glm::vec2(5.f, 0.3f), pos + glm::vec2(4.7f, 0.3f), getColor());
    px::Renderer::DrawTri(pos + glm::vec2(5.f, 0.f), pos + glm::vec2(5.f, 0.3f), pos + glm::vec2(4.7f, 0.3f));

    for (auto&& output : outputs) {
      px::Renderer::DrawLine(
          pos + (size * glm::vec2 {1.f, 0.5f}), output->pos + (size * glm::vec2 {0.f, 0.5f}), 0.1f, getColor());
    }
  }

  InputGate::InputGate() : LogicGate() { size = glm::vec2 {10.f, 10.f}; }
  InputGate::InputGate(const px::UUID& uuid) : LogicGate(uuid) { size = glm::vec2 {10.f, 10.f}; }

  void InputGate::UpdateState() {}

  void InputGate::Draw() {
    px::Renderer::BorderCircle(pos + glm::vec2(5.f, 5.f), 5.f, 30, 0.3f, getColor());

    for (auto&& output : outputs) {
      px::Renderer::DrawLine(pos + glm::vec2(10.f, 5.f), output->pos + glm::vec2(0.f, 5.f), 0.1f, getColor());
    }
  }

  OutputGate::OutputGate() : LogicGate() { size = glm::vec2 {10.f, 10.f}; }
  OutputGate::OutputGate(const px::UUID& uuid) : LogicGate(uuid) { size = glm::vec2 {10.f, 10.f}; }

  void OutputGate::UpdateState() {
    if (inputs.size() != 1) {
      state = State::ERROR;
    } else {
      state = inputs.begin()->get()->state;
    }
  }

  void OutputGate::Draw() { px::Renderer::BorderCircle(pos + glm::vec2(5.f, 5.f), 5.f, 30, 0.3f, getColor()); }
}
