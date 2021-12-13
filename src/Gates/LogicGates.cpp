#include "Gates/LogicGates.hpp"

namespace Gates {
  void NotGate::UpdateState() {
    if (inputs.size() != 1 || inputs.at(0) == State::ERROR) {
      state = State::ERROR;
    } else if (inputs.at(0) == State::OFF) {
      state = State::ON;
    } else {
      state = State::OFF;
    }
  }

  void NotGate::DrawGate() {}

  void AndGate::UpdateState() {
    if (inputs.size() < 2) {
      state = State::ERROR;
    } else {
      uint32_t count = 0;

      for (const State& input : inputs) {
        switch (input) {
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

  void AndGate::DrawGate() {}

  void OrGate::UpdateState() {
    if (inputs.size() < 2) {
      state = State::ERROR;
    } else {
      for (const State& input : inputs) {
        switch (input) {
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

  void OrGate::DrawGate() {}

  void XorGate::UpdateState() {
    if (inputs.size() < 2) {
      state = State::ERROR;
    } else {
      uint32_t count = 0;

      for (const State& input : inputs) {
        switch (input) {
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

  void XorGate::DrawGate() {}

  void InputGate::UpdateState() {}
  void InputGate::DrawGate() {}

  void OutputGate::UpdateState() {}
  void OutputGate::DrawGate() {}
}
