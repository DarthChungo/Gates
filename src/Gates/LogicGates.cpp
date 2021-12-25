#include "Gates/LogicGates.hpp"
#include "Engine/Renderer.hpp"

namespace Gates {
  NotGate::NotGate() : LogicGate() { size = glm::vec2 {8.f, 10.f}; }

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
    switch (state) {
      case State::ERROR:
        Renderer::DrawQuad(pos, size, {1.f, 0.f, 0.f, 1.f});
        break;

      case State::ON:
        Renderer::DrawQuad(pos, size, {0.f, 1.f, 0.f, 1.f});
        break;

      case State::OFF:
        Renderer::DrawQuad(pos, size, {0.3f, 0.3f, 0.3f, 1.f});
        break;
    }

    for (auto&& output : outputs) {
      switch (state) {
        case State::ERROR:
          Renderer::DrawLine(pos + (size * glm::vec2 {1.f, 0.5f}),
                             output->pos + (size * glm::vec2 {0.f, 0.5f}),
                             0.1f,
                             {1.f, 0.f, 0.f, 1.f});
          break;

        case State::ON:
          Renderer::DrawLine(pos + (size * glm::vec2 {1.f, 0.5f}),
                             output->pos + (size * glm::vec2 {0.f, 0.5f}),
                             0.1f,
                             {0.f, 1.f, 0.f, 1.f});
          break;

        case State::OFF:
          Renderer::DrawLine(pos + (size * glm::vec2 {1.f, 0.5f}),
                             output->pos + (size * glm::vec2 {0.f, 0.5f}),
                             0.1f,
                             {0.3f, 0.3f, 0.3f, 1.f});
          break;
      }
    }
  }

  AndGate::AndGate() : LogicGate() { size = glm::vec2 {10.f, 10.f}; }

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
    switch (state) {
      case State::ERROR:
        Renderer::DrawQuad(pos, size, {1.f, 0.f, 0.f, 1.f});
        break;

      case State::ON:
        Renderer::DrawQuad(pos, size, {0.f, 1.f, 0.f, 1.f});
        break;

      case State::OFF:
        Renderer::DrawQuad(pos, size, {0.3f, 0.3f, 0.3f, 1.f});
        break;
    }

    for (auto&& output : outputs) {
      switch (state) {
        case State::ERROR:
          Renderer::DrawLine(pos + (size * glm::vec2 {1.f, 0.5f}),
                             output->pos + (size * glm::vec2 {0.f, 0.5f}),
                             0.1f,
                             {1.f, 0.f, 0.f, 1.f});
          break;

        case State::ON:
          Renderer::DrawLine(pos + (size * glm::vec2 {1.f, 0.5f}),
                             output->pos + (size * glm::vec2 {0.f, 0.5f}),
                             0.1f,
                             {0.f, 1.f, 0.f, 1.f});
          break;

        case State::OFF:
          Renderer::DrawLine(pos + (size * glm::vec2 {1.f, 0.5f}),
                             output->pos + (size * glm::vec2 {0.f, 0.5f}),
                             0.1f,
                             {0.3f, 0.3f, 0.3f, 1.f});
          break;
      }
    }
  }

  OrGate::OrGate() : LogicGate() { size = glm::vec2 {10.f, 10.f}; }

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
    switch (state) {
      case State::ERROR:
        Renderer::DrawQuad(pos, size, {1.f, 0.f, 0.f, 1.f});
        break;

      case State::ON:
        Renderer::DrawQuad(pos, size, {0.f, 1.f, 0.f, 1.f});
        break;

      case State::OFF:
        Renderer::DrawQuad(pos, size, {0.3f, 0.3f, 0.3f, 1.f});
        break;
    }

    for (auto&& output : outputs) {
      switch (state) {
        case State::ERROR:
          Renderer::DrawLine(pos + (size * glm::vec2 {1.f, 0.5f}),
                             output->pos + (size * glm::vec2 {0.f, 0.5f}),
                             0.1f,
                             {1.f, 0.f, 0.f, 1.f});
          break;

        case State::ON:
          Renderer::DrawLine(pos + (size * glm::vec2 {1.f, 0.5f}),
                             output->pos + (size * glm::vec2 {0.f, 0.5f}),
                             0.1f,
                             {0.f, 1.f, 0.f, 1.f});
          break;

        case State::OFF:
          Renderer::DrawLine(pos + (size * glm::vec2 {1.f, 0.5f}),
                             output->pos + (size * glm::vec2 {0.f, 0.5f}),
                             0.1f,
                             {0.3f, 0.3f, 0.3f, 1.f});
          break;
      }
    }
  }

  XorGate::XorGate() : LogicGate() { size = glm::vec2 {10.f, 10.f}; }

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
    switch (state) {
      case State::ERROR:
        Renderer::DrawQuad(pos, size, {1.f, 0.f, 0.f, 1.f});
        break;

      case State::ON:
        Renderer::DrawQuad(pos, size, {0.f, 1.f, 0.f, 1.f});
        break;

      case State::OFF:
        Renderer::DrawQuad(pos, size, {0.3f, 0.3f, 0.3f, 1.f});
        break;
    }

    for (auto&& output : outputs) {
      switch (state) {
        case State::ERROR:
          Renderer::DrawLine(pos + (size * glm::vec2 {1.f, 0.5f}),
                             output->pos + (size * glm::vec2 {0.f, 0.5f}),
                             0.1f,
                             {1.f, 0.f, 0.f, 1.f});
          break;

        case State::ON:
          Renderer::DrawLine(pos + (size * glm::vec2 {1.f, 0.5f}),
                             output->pos + (size * glm::vec2 {0.f, 0.5f}),
                             0.1f,
                             {0.f, 1.f, 0.f, 1.f});
          break;

        case State::OFF:
          Renderer::DrawLine(pos + (size * glm::vec2 {1.f, 0.5f}),
                             output->pos + (size * glm::vec2 {0.f, 0.5f}),
                             0.1f,
                             {0.3f, 0.3f, 0.3f, 1.f});
          break;
      }
    }
  }

  Texture InputGate::on_tex;
  Texture InputGate::off_tex;
  Texture InputGate::err_tex;

  uint64_t InputGate::count = 0;

  InputGate::InputGate() : LogicGate() {
    size = glm::vec2 {10.f, 10.f};

    if (count++ == 0) {
      on_tex.Load("assets/gates/input_on.png");
      off_tex.Load("assets/gates/input_off.png");
      err_tex.Load("assets/gates/input_err.png");
    }
  }

  InputGate::~InputGate() {
    if (--count == 0) {
      on_tex.Release();
      off_tex.Release();
      err_tex.Release();
    }
  }

  void InputGate::UpdateState() {}

  void InputGate::Draw() {
    switch (state) {
      case State::ERROR:
        Renderer::DrawQuad(pos, size, {1.f, 1.f, 1.f, 1.f}, err_tex);
        break;

      case State::ON:
        Renderer::DrawQuad(pos, size, {1.f, 1.f, 1.f, 1.f}, on_tex);
        break;

      case State::OFF:
        Renderer::DrawQuad(pos, size, {1.f, 1.f, 1.f, 1.f}, off_tex);
        break;
    }

    for (auto&& output : outputs) {
      switch (state) {
        case State::ERROR:
          Renderer::DrawLine(pos + (size * glm::vec2 {1.f, 0.5f}),
                             output->pos + (size * glm::vec2 {0.f, 0.5f}),
                             0.1f,
                             {1.f, 0.f, 0.f, 1.f});
          break;

        case State::ON:
          Renderer::DrawLine(pos + (size * glm::vec2 {1.f, 0.5f}),
                             output->pos + (size * glm::vec2 {0.f, 0.5f}),
                             0.1f,
                             {0.f, 1.f, 0.f, 1.f});
          break;

        case State::OFF:
          Renderer::DrawLine(pos + (size * glm::vec2 {1.f, 0.5f}),
                             output->pos + (size * glm::vec2 {0.f, 0.5f}),
                             0.1f,
                             {0.3f, 0.3f, 0.3f, 1.f});
          break;
      }
    }
  }

  Texture OutputGate::on_tex;
  Texture OutputGate::off_tex;
  Texture OutputGate::err_tex;

  uint64_t OutputGate::count = 0;

  OutputGate::OutputGate() : LogicGate() {
    size = glm::vec2 {10.f, 10.f};

    if (count++ == 0) {
      on_tex.Load("assets/gates/output_on.png");
      off_tex.Load("assets/gates/output_off.png");
      err_tex.Load("assets/gates/output_err.png");
    }
  }

  OutputGate::~OutputGate() {
    if (--count == 0) {
      on_tex.Release();
      off_tex.Release();
      err_tex.Release();
    }
  }

  void OutputGate::UpdateState() {
    if (inputs.size() != 1) {
      state = State::ERROR;
    } else {
      state = inputs.begin()->get()->state;
    }
  }

  void OutputGate::Draw() {
    switch (state) {
      case State::ERROR:
        Renderer::DrawQuad(pos, size, {1.f, 1.f, 1.f, 1.f}, err_tex);
        break;

      case State::ON:
        Renderer::DrawQuad(pos, size, {1.f, 1.f, 1.f, 1.f}, on_tex);
        break;

      case State::OFF:
        Renderer::DrawQuad(pos, size, {1.f, 1.f, 1.f, 1.f}, off_tex);
        break;
    }

    for (auto&& output : outputs) {
      switch (state) {
        case State::ERROR:
          Renderer::DrawLine(pos + (size * glm::vec2 {1.f, 0.5f}),
                             output->pos + (size * glm::vec2 {0.f, 0.5f}),
                             0.1f,
                             {1.f, 0.f, 0.f, 1.f});
          break;

        case State::ON:
          Renderer::DrawLine(pos + (size * glm::vec2 {1.f, 0.5f}),
                             output->pos + (size * glm::vec2 {0.f, 0.5f}),
                             0.1f,
                             {0.f, 1.f, 0.f, 1.f});
          break;

        case State::OFF:
          Renderer::DrawLine(pos + (size * glm::vec2 {1.f, 0.5f}),
                             output->pos + (size * glm::vec2 {0.f, 0.5f}),
                             0.1f,
                             {0.3f, 0.3f, 0.3f, 1.f});
          break;
      }
    }
  }
}
