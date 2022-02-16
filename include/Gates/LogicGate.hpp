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

#ifndef GATES_LOGICGATE_HPP
#define GATES_LOGICGATE_HPP

#include "Gates/include.hpp"

namespace Gates {
  enum class State : uint32_t { ON, OFF, ERROR };
  static std::unordered_map<State, const char*> StateReadableNames = {{State::ON, "ON"},
                                                                      {State::OFF, "OFF"},
                                                                      {State::ERROR, "ERROR"}};

  class LogicGate {
   public:
    LogicGate();
    LogicGate(const px::UUID& custom_id);

    virtual void UpdateState() = 0;
    virtual void Draw()        = 0;

    virtual const char* getName() = 0;
    const glm::vec4&    getColor();

    static constexpr glm::vec4 on_color  = {0.545f, 0.765f, 0.29f, 1.f};
    static constexpr glm::vec4 off_color = {0.3f, 0.3f, 0.3f, 1.f};
    static constexpr glm::vec4 err_color = {0.769f, 0.09f, 0.0f, 1.f};

   public:
    std::set<std::shared_ptr<LogicGate>> inputs;
    std::set<std::shared_ptr<LogicGate>> outputs;

    State    state = State::ERROR;
    px::UUID id;

    glm::vec2 pos;
    glm::vec2 size;
  };
}

#endif
