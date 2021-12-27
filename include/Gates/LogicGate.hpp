#ifndef GATES_LOGICGATE_HPP
#define GATES_LOGICGATE_HPP

#include "pch.hpp"
#include "Util/UUID.hpp"

namespace Gates {
  enum class State : uint32_t { ON, OFF, ERROR };
  static std::unordered_map<State, const char*> StateNames = {{State::ON, "ON"},
                                                              {State::OFF, "OFF"},
                                                              {State::ERROR, "ERROR"}};

  class LogicGate {
   public:
    LogicGate();

    virtual void        UpdateState() = 0;
    virtual void        Draw()        = 0;
    virtual const char* getName()     = 0;

    static constexpr glm::vec4 on_color  = {0.f, 1.f, 0.f, 1.f};
    static constexpr glm::vec4 off_color = {0.3f, 0.3f, 0.3f, 1.f};
    static constexpr glm::vec4 err_color = {1.f, 0.f, 0.f, 1.f};

   public:
    std::set<std::shared_ptr<LogicGate>> inputs;
    std::set<std::shared_ptr<LogicGate>> outputs;

    State state = State::ERROR;
    UUID  id;

    glm::vec2 pos;
    glm::vec2 size;
  };
}

#endif
