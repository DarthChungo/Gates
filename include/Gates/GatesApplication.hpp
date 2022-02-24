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
    px::rcode pOnUpdate() override;       // Se ejecuta en cada actualización gráfica
    px::rcode pOnRender() override;       // Se ejecuta en cada actualización gráfica
    px::rcode pOnImGuiRender() override;  // Se ejecuta al renderizar la interfáz gráfica
    px::rcode pOnLaunch() override;       // Se ejecuta al iniciar la aplicación
    px::rcode pOnClose() override;        // Se ejecuta al cerrar la aplicación

    glm::vec2 MousePosWorld();

   private:
    bool dragging = false;

    glm::vec2 offset = {};

    float                  view_distance = 60.f;
    float                  view_rotation = 0.f;
    px::OrthographicCamera camera {view_distance,
                                   1.f};  // Anidación con la clase OrthographicCamera

    bool show_truthtable = true;
    bool show_controls   = true;

    LogicCircuit circuit {this};  // Anicación con la clase LogicCircuit

    // También se realizan anidaciones indirectas con clases estáticas, que no tienen instancias,
    // pero cuyo ciclo de vida depende de esta clase. Ejemplo: Pixel::Renderer y
    // Gates::DataSerializer
  };
}

#endif
