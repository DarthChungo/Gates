#include "Gates/LogicGate.hpp"

namespace Gates {
  LogicGate::LogicGate() : id {} {};
  LogicGate::LogicGate(const px::UUID& custom_id) : id(custom_id) {};

  // Color dado el estado
  const glm::vec4& LogicGate::getColor() {
    return state == State::OFF ? off_color : (state == State::ON ? on_color : err_color);
  }
}
