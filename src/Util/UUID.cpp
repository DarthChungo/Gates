#include "Util/UUID.hpp"

namespace Gates {
  static std::random_device                      random_device;
  static std::mt19937_64                         engine(random_device());
  static std::uniform_int_distribution<uint64_t> uniform_ditribution;

  UUID::UUID() : pUUID(uniform_ditribution(engine)) {}

  UUID::UUID(uint64_t uuid) : pUUID(uuid) {}
}
