#ifndef GATES_LOGICGATE_HPP
#define GATES_LOGICGATE_HPP

#include "Pixel/Pixel.hpp"

namespace px = Pixel;

namespace Gates {
  enum class State : uint32_t { ON, OFF, ERROR };
  static std::unordered_map<State, const char*> StateReadableNames = {{State::ON, "ON"},
                                                                      {State::OFF, "OFF"},
                                                                      {State::ERROR, "ERROR"}};

  // Clase abstracta que representa una puerta lógica
  class LogicGate {
   public:
    LogicGate();                           // Construir con un id aleatorio
    LogicGate(const px::UUID& custom_id);  // Construir con un id dado

    virtual void UpdateState() = 0;
    virtual void Draw()        = 0;

    virtual const char* getName() = 0;
    const glm::vec4&    getColor();

    // Colores predeterminados

    static constexpr glm::vec4 on_color  = {0.545f, 0.765f, 0.29f, 1.f};
    static constexpr glm::vec4 off_color = {0.3f, 0.3f, 0.3f, 1.f};
    static constexpr glm::vec4 err_color = {0.769f, 0.09f, 0.0f, 1.f};

   public:
    std::set<std::shared_ptr<LogicGate>> inputs;   // Referencias a las entradas
    std::set<std::shared_ptr<LogicGate>> outputs;  // Referencias a las salidas

    State    state = State::ERROR;  // Estado interno
    px::UUID id;                    // Id interno

    glm::vec2 pos;   // Posición en el circuito
    glm::vec2 size;  // Tamaño de la puerta
  };
}

#endif
