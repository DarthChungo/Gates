#include "Gates/LogicGates.hpp"
#include "Engine/Renderer.hpp"

namespace Gates {
  NotGate::NotGate() : LogicGate() { size = glm::vec2 {10.f, 10.f}; }

  void NotGate::UpdateState() {
    if (inputs.size() != 1 || inputs.begin()->get()->state == State::ERROR) {
      state = State::ERROR;

    } else if (inputs.begin()->get()->state == State::OFF) {
      state = State::ON;

    } else {
      state = State::OFF;
    }
  }

  void NotGate::Draw(bool sel) {
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

    if (sel) Renderer::OutlineQuad(pos, size);

    for (auto&& output : outputs) {
      switch (state) {
        case State::ERROR:
          Renderer::DrawLine(
              pos + (size * glm::vec2 {1.f, 0.5f}), output->pos + (size * glm::vec2 {0.f, 0.5f}), {1.f, 0.f, 0.f, 1.f});
          break;

        case State::ON:
          Renderer::DrawLine(
              pos + (size * glm::vec2 {1.f, 0.5f}), output->pos + (size * glm::vec2 {0.f, 0.5f}), {0.f, 1.f, 0.f, 1.f});
          break;

        case State::OFF:
          Renderer::DrawLine(pos + (size * glm::vec2 {1.f, 0.5f}),
                             output->pos + (size * glm::vec2 {0.f, 0.5f}),
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

  void AndGate::Draw(bool sel) {
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

    if (sel) Renderer::OutlineQuad(pos, size);

    for (auto&& output : outputs) {
      switch (state) {
        case State::ERROR:
          Renderer::DrawLine(
              pos + (size * glm::vec2 {1.f, 0.5f}), output->pos + (size * glm::vec2 {0.f, 0.5f}), {1.f, 0.f, 0.f, 1.f});
          break;

        case State::ON:
          Renderer::DrawLine(
              pos + (size * glm::vec2 {1.f, 0.5f}), output->pos + (size * glm::vec2 {0.f, 0.5f}), {0.f, 1.f, 0.f, 1.f});
          break;

        case State::OFF:
          Renderer::DrawLine(pos + (size * glm::vec2 {1.f, 0.5f}),
                             output->pos + (size * glm::vec2 {0.f, 0.5f}),
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

  void OrGate::Draw(bool sel) {
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

    if (sel) Renderer::OutlineQuad(pos, size);

    for (auto&& output : outputs) {
      switch (state) {
        case State::ERROR:
          Renderer::DrawLine(
              pos + (size * glm::vec2 {1.f, 0.5f}), output->pos + (size * glm::vec2 {0.f, 0.5f}), {1.f, 0.f, 0.f, 1.f});
          break;

        case State::ON:
          Renderer::DrawLine(
              pos + (size * glm::vec2 {1.f, 0.5f}), output->pos + (size * glm::vec2 {0.f, 0.5f}), {0.f, 1.f, 0.f, 1.f});
          break;

        case State::OFF:
          Renderer::DrawLine(pos + (size * glm::vec2 {1.f, 0.5f}),
                             output->pos + (size * glm::vec2 {0.f, 0.5f}),
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

  void XorGate::Draw(bool sel) {
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

    if (sel) Renderer::OutlineQuad(pos, size);

    for (auto&& output : outputs) {
      switch (state) {
        case State::ERROR:
          Renderer::DrawLine(
              pos + (size * glm::vec2 {1.f, 0.5f}), output->pos + (size * glm::vec2 {0.f, 0.5f}), {1.f, 0.f, 0.f, 1.f});
          break;

        case State::ON:
          Renderer::DrawLine(
              pos + (size * glm::vec2 {1.f, 0.5f}), output->pos + (size * glm::vec2 {0.f, 0.5f}), {0.f, 1.f, 0.f, 1.f});
          break;

        case State::OFF:
          Renderer::DrawLine(pos + (size * glm::vec2 {1.f, 0.5f}),
                             output->pos + (size * glm::vec2 {0.f, 0.5f}),
                             {0.3f, 0.3f, 0.3f, 1.f});
          break;
      }
    }
  }

  Texture InputGate::on_tex;
  Texture InputGate::off_tex;
  Texture InputGate::err_tex;
  Texture InputGate::on_tex_sel;
  Texture InputGate::off_tex_sel;
  Texture InputGate::err_tex_sel;

  uint64_t InputGate::count = 0;

  InputGate::InputGate() : LogicGate() {
    size = glm::vec2 {10.f, 10.f};

    if (count++ == 0) {
      on_tex.Load("assets/gates/input_on.png");
      off_tex.Load("assets/gates/input_off.png");
      err_tex.Load("assets/gates/input_err.png");
      on_tex_sel.Load("assets/gates/input_on_sel.png");
      off_tex_sel.Load("assets/gates/input_off_sel.png");
      err_tex_sel.Load("assets/gates/input_err_sel.png");
    }
  }

  InputGate::~InputGate() {
    if (--count == 0) {
      on_tex.Release();
      off_tex.Release();
      err_tex.Release();
      on_tex_sel.Release();
      off_tex_sel.Release();
      err_tex_sel.Release();
    }
  }

  void InputGate::UpdateState() {}

  void InputGate::Draw(bool sel) {
    switch (state) {
      case State::ERROR:
        if (sel)
          Renderer::DrawQuad(pos, size, {1.f, 1.f, 1.f, 1.f}, err_tex_sel);
        else
          Renderer::DrawQuad(pos, size, {1.f, 1.f, 1.f, 1.f}, err_tex);
        break;

      case State::ON:
        if (sel)
          Renderer::DrawQuad(pos, size, {1.f, 1.f, 1.f, 1.f}, on_tex_sel);
        else
          Renderer::DrawQuad(pos, size, {1.f, 1.f, 1.f, 1.f}, on_tex);
        break;

      case State::OFF:
        if (sel)
          Renderer::DrawQuad(pos, size, {1.f, 1.f, 1.f, 1.f}, off_tex_sel);
        else
          Renderer::DrawQuad(pos, size, {1.f, 1.f, 1.f, 1.f}, off_tex);
        break;
    }

    for (auto&& output : outputs) {
      switch (state) {
        case State::ERROR:
          Renderer::DrawLine(
              pos + (size * glm::vec2 {1.f, 0.5f}), output->pos + (size * glm::vec2 {0.f, 0.5f}), {1.f, 0.f, 0.f, 1.f});
          break;

        case State::ON:
          Renderer::DrawLine(
              pos + (size * glm::vec2 {1.f, 0.5f}), output->pos + (size * glm::vec2 {0.f, 0.5f}), {0.f, 1.f, 0.f, 1.f});
          break;

        case State::OFF:
          Renderer::DrawLine(pos + (size * glm::vec2 {1.f, 0.5f}),
                             output->pos + (size * glm::vec2 {0.f, 0.5f}),
                             {0.3f, 0.3f, 0.3f, 1.f});
          break;
      }
    }
  }

  Texture OutputGate::on_tex;
  Texture OutputGate::off_tex;
  Texture OutputGate::err_tex;
  Texture OutputGate::on_tex_sel;
  Texture OutputGate::off_tex_sel;
  Texture OutputGate::err_tex_sel;

  uint64_t OutputGate::count = 0;

  OutputGate::OutputGate() : LogicGate() {
    size = glm::vec2 {10.f, 10.f};

    if (count++ == 0) {
      on_tex.Load("assets/gates/output_on.png");
      off_tex.Load("assets/gates/output_off.png");
      err_tex.Load("assets/gates/output_err.png");
      on_tex_sel.Load("assets/gates/output_on_sel.png");
      off_tex_sel.Load("assets/gates/output_off_sel.png");
      err_tex_sel.Load("assets/gates/output_err_sel.png");
    }
  }

  OutputGate::~OutputGate() {
    if (--count == 0) {
      on_tex.Release();
      off_tex.Release();
      err_tex.Release();
      on_tex_sel.Release();
      off_tex_sel.Release();
      err_tex_sel.Release();
    }
  }

  void OutputGate::UpdateState() {
    if (inputs.size() != 1) {
      state = State::ERROR;
    } else {
      state = inputs.begin()->get()->state;
    }
  }

  void OutputGate::Draw(bool sel) {
    switch (state) {
      case State::ERROR:
        if (sel)
          Renderer::DrawQuad(pos, size, {1.f, 1.f, 1.f, 1.f}, err_tex_sel);
        else
          Renderer::DrawQuad(pos, size, {1.f, 1.f, 1.f, 1.f}, err_tex);
        break;

      case State::ON:
        if (sel)
          Renderer::DrawQuad(pos, size, {1.f, 1.f, 1.f, 1.f}, on_tex_sel);
        else
          Renderer::DrawQuad(pos, size, {1.f, 1.f, 1.f, 1.f}, on_tex);
        break;

      case State::OFF:
        if (sel)
          Renderer::DrawQuad(pos, size, {1.f, 1.f, 1.f, 1.f}, off_tex_sel);
        else
          Renderer::DrawQuad(pos, size, {1.f, 1.f, 1.f, 1.f}, off_tex);
        break;
    }

    for (auto&& output : outputs) {
      switch (state) {
        case State::ERROR:
          Renderer::DrawLine(
              pos + (size * glm::vec2 {1.f, 0.5f}), output->pos + (size * glm::vec2 {0.f, 0.5f}), {1.f, 0.f, 0.f, 1.f});
          break;

        case State::ON:
          Renderer::DrawLine(
              pos + (size * glm::vec2 {1.f, 0.5f}), output->pos + (size * glm::vec2 {0.f, 0.5f}), {0.f, 1.f, 0.f, 1.f});
          break;

        case State::OFF:
          Renderer::DrawLine(pos + (size * glm::vec2 {1.f, 0.5f}),
                             output->pos + (size * glm::vec2 {0.f, 0.5f}),
                             {0.3f, 0.3f, 0.3f, 1.f});
          break;
      }
    }
  }
}
