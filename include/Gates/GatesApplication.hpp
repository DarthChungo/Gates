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

#include "Pixel/Pixel.hpp"
#include "Gates/LogicCircuit.hpp"

namespace px = Pixel;

#define GATES_APPLICATION_VERSION_TXT "Versión 1.0"

namespace Gates {
  // Clase principal, herencia pública de Pixel::Application
  class GatesApplication : public px::Application {
   public:
    // EL uso de friend permite acceder a los miembros provados desde otra clase
    friend class LogicCircuit;

   private:
    // Eventos principales
    // El uso de override sobreescribe el código de la clase padre para ejecutal el código propio
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
    px::OrthographicCamera camera {view_distance, 1.f};  // Anidación con la clase OrthographicCamera

    bool show_truthtable = true;
    bool show_controls   = true;

    LogicCircuit circuit {this};  // Anicación con la clase LogicCircuit

    // También se realizan anidaciones indirectas con clases estáticas, que no tienen instancias, pero cuyo ciclo de
    // vida depende de esta clase. Ejemplo: Pixel::Renderer y Gates::DataSerializer
  };
}

#define GATES_APPLICATION_LICENSE                                          \
  "Gates, a simple logic circuit simulator written in C++\n"               \
  "Copyright (C) 2022 DarthChungo\n"                                       \
  "\n"                                                                     \
  "This program is free software: you can redistribute it and/or modify\n" \
  "it under the terms of the GNU General Public License as published by\n" \
  "the Free Software Foundation, either version 3 of the License, or\n"    \
  "(at your option) any later version.\n"                                  \
  "\n"                                                                     \
  "This program is distributed in the hope that it will be useful,\n"      \
  "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"       \
  "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"        \
  "GNU General Public License for more details.\n"                         \
  "\n"                                                                     \
  "You should have received a copy of the GNU General Public License\n"    \
  "along with this program.  If not, see <https://www.gnu.org/licenses/>.\n"

#endif
