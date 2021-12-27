#include "Gates/LogicGate.hpp"

namespace Gates {
  LogicGate::LogicGate() : id {} {};

  const glm::vec4& LogicGate::getColor() {
    return state == State::OFF ? off_color : (state == State::ON ? on_color : err_color);
  }
}
