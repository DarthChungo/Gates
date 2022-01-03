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

namespace std {
  template <>
  struct hash<Gates::UUID> {
    inline std::size_t operator()(const Gates::UUID& uuid) const { return hash<uint64_t>()((uint64_t)uuid); }
  };
}

#endif
