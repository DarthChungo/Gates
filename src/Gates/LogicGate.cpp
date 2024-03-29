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

#include "Gates/LogicGate.hpp"

namespace Gates {
  LogicGate::LogicGate() : id {} {};
  LogicGate::LogicGate(const px::UUID& custom_id) : id(custom_id) {};

  const glm::vec4& LogicGate::getColor() {
    return state == State::OFF ? off_color : (state == State::ON ? on_color : err_color);
  }
}
