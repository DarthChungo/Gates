#ifndef GATES_LOGICCIRCUIT_HPP
#define GATES_LOGICCIRCUIT_HPP

#include "Pixel/Pixel.hpp"
#include "Gates/LogicGates.hpp"
#include "Gates/LogicGate.hpp"

namespace px = Pixel;

namespace Gates {
  class GatesApplication;

  // Clase principal de circuito lógico
  class LogicCircuit {
   public:
    using TruthTableEntry = std::pair<std::vector<bool>, std::vector<bool>>;
    using TruthTable      = std::vector<TruthTableEntry>;

    LogicCircuit(GatesApplication* application) : app(application) {}

    // Esta función se tiene que definir en el encabezado, en vez de en el archivo cpp por
    // limitaciones del vinculador de C++ Añade una puerta al circuito mediante el ratón del usuario
    template <class GateType>
    std::shared_ptr<LogicGate> AddGate() requires std::derived_from<GateType, LogicGate> {
      auto new_gate = std::make_shared<GateType>();  // Alojamiento de menoria
      gates.insert(new_gate);

      adding      = true;
      adding_gate = new_gate;

      // Si la puerta es una puerta de entrada, hay que añadírla a una lista especial
      if constexpr (std::is_same<GateType, InputGate>::value) {
        gates_input.insert(new_gate);
        new_gate->state = State::OFF;
      } else if constexpr (std::is_same<GateType, OutputGate>::value) {
        gates_output.insert(new_gate);
      }

      return new_gate;
    }

    // Esta función se tiene que definir en el encabezado, en vez de en el archivo cpp por
    // limitaciones del vinculador de C++ Añade una puerta al circuito dada una posición e
    // identificador
    template <class GateType>
    std::shared_ptr<LogicGate> AddGateDirect(const glm::vec2 pos, const px::UUID& uuid) requires
        std::derived_from<GateType, LogicGate> {
      auto new_gate = std::make_shared<GateType>(uuid);  // Alojamiento de menoria
      new_gate->pos = pos;
      gates.insert(new_gate);

      // Si la puerta es una puerta de entrada, hay que añadírla a una lista especial
      if constexpr (std::is_same<GateType, InputGate>::value) {
        gates_input.insert(new_gate);
        new_gate->state = State::OFF;
      } else if constexpr (std::is_same<GateType, OutputGate>::value) {
        gates_output.insert(new_gate);
      }

      return new_gate;
    }

    void UpdateLogicState();     // Actualizar el estado lógico
    void UpdateGraphicsState();  // Actualizar la interfáz gráfica, las posiciones de las puertas y
                                 // sus conexiones
    void DrawGates();  // Dibujar todas las puertas y sus conexiones

    void SetInput(const std::shared_ptr<LogicGate>& input,
                  State                             val);  // Alternar una entrada al circuito
    void ToggleConnection(
        const std::shared_ptr<LogicGate>& from,
        const std::shared_ptr<LogicGate>& to);  // Alternar una conexión entre puertas
    void MakeConnection(const px::UUID& from,
                        const px::UUID& to);  // Hacer una conexión dados dos identificadores

    TruthTable ComputeTruthTable();

   public:
    std::set<std::shared_ptr<LogicGate>> gates;  // Guarda referencias a todas las puertas
    std::set<std::shared_ptr<LogicGate>>
        gates_update_pending;  // Guarda referencia a todas las puertas por actualizar todavía
    std::set<std::shared_ptr<LogicGate>>
        gates_update_forced;  // Guarda referencia a todas las puertas recien añadidas
    std::set<std::shared_ptr<LogicGate>>
        gates_input;  // Guarda referencia a todas las puertas input
    std::set<std::shared_ptr<LogicGate>>
        gates_output;  // Guarda referencia a todas las puertas output

    // Las siguientes variables guardan referencias a puertas especiales en el circuito

    std::shared_ptr<LogicGate> hovered_gate;   // Puerta debajo del ratón
    std::shared_ptr<LogicGate> selected_gate;  // Puerta seleccionada
    std::shared_ptr<LogicGate> dragging_gate;  // Puerta siendo arrastrada
    glm::vec2                  dragging_offset = {};
    bool                       dragging        = false;

    std::shared_ptr<LogicGate> adding_gate;  // Puerta siendo añadida
    bool                       adding = false;

    std::shared_ptr<LogicGate> outputing_gate;  // Puerta con la que se está creando una conexión
    bool                       outputing = false;

    GatesApplication* const app;
  };
}

#endif
