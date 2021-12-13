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
    friend class LogicCircuit;

    explicit LogicGate() : id {} {};
    virtual ~LogicGate() {};

    virtual void        UpdateState() = 0;
    virtual void        DrawGate()    = 0;
    virtual const char* getName()     = 0;

   public:
    std::vector<std::shared_ptr<LogicGate>> inputs;
    std::vector<std::shared_ptr<LogicGate>> outputs;

    State state = State::ERROR;
    UUID  id;
  };
}

#endif
