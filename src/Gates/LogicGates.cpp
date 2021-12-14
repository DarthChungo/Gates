#include "Gates/LogicGates.hpp"

namespace Gates {
  void NotGate::UpdateState() {
    if (inputs.size() != 1 || inputs.begin()->get()->state == State::ERROR) {
      state = State::ERROR;

    } else if (inputs.begin()->get()->state == State::OFF) {
      state = State::ON;

    } else {
      state = State::OFF;
    }
  }

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

  void InputGate::UpdateState() {}

  void OutputGate::UpdateState() {
    if (inputs.size() != 1) {
      state = State::ERROR;
    } else {
      state = inputs.begin()->get()->state;
    }
  }
}
