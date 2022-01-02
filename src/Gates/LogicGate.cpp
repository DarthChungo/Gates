#include "Gates/LogicGate.hpp"

namespace Gates {
  LogicGate::LogicGate() : id {} {};
  LogicGate::LogicGate(const UUID& custom_id) : id(custom_id) {};

  const glm::vec4& LogicGate::getColor() {
    return state == State::OFF ? off_color : (state == State::ON ? on_color : err_color);
  }
}
