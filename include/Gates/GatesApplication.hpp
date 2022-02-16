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

#ifndef GATES_GATESAPPLICATION_HPP
#define GATES_GATESAPPLICATION_HPP

#include "Gates/include.hpp"
#include "Gates/LogicCircuit.hpp"

#define GATES_APPLICATION_VERSION_TXT "Versión 1.0"

namespace Gates {
  class GatesApplication : public px::Application {
   public:
    friend class LogicCircuit;

   private:
    px::rcode pOnUpdate() override;
    px::rcode pOnRender() override;
    px::rcode pOnImGuiRender() override;
    px::rcode pOnLaunch() override;
    px::rcode pOnClose() override;

    glm::vec2 MousePosWorld();

   private:
    bool dragging = false;

    glm::vec2 offset = {};

    float                  view_distance = 60.f;
    float                  view_rotation = 0.f;
    px::OrthographicCamera camera {view_distance, 1.f};

    bool show_statistics = false;
    bool show_controls   = true;

    LogicCircuit circuit {this};
  };
}

#endif
