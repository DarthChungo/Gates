#include "Gates/LogicCircuit.hpp"
#include "Gates/LogicGates.hpp"
#include "Util/Misc.hpp"

namespace Gates {
  void LogicCircuit::UpdateState() {
    std::set<std::shared_ptr<LogicGate>> gates_update_pending;
    std::set<std::shared_ptr<LogicGate>> gates_updating;

    for (auto&& gate : gates) {  // FIXME: hack
      if (instanceof <InputGate>(gate.get())) {
        gates_update_pending.insert(gate);
      }
    }

    while (!gates_update_pending.empty()) {
      gates_updating = gates_update_pending;
      gates_update_pending.clear();

      for (auto&& gate : gates_updating) {
        State previous_state = gate->state;
        gate->UpdateState();

        if (gate->state != previous_state || /*FIXME: hack*/ instanceof <InputGate>(gate.get())) {
          for (auto&& child : gate->outputs) {
            gates_update_pending.insert(child);
          }
        }
      }
    }
  }
}
