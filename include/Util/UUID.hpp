#ifndef GATES_UUID_HPP
#define GATES_UUID_HPP

#include "pch.hpp"

namespace Gates {
  class UUID {  // Not really an UUID
   public:
    UUID();
    UUID(uint64_t uuid);

    operator uint64_t() const { return pUUID; }

   private:
    uint64_t pUUID;
  };
}

#endif
