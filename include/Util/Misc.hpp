#ifndef GATES_MISC_HPP
#define GATES_MISC_HPP

#include "pch.hpp"

namespace Gates {
  template <typename Base, typename T>
  inline bool instanceof (const T *ptr) {
    return dynamic_cast<const Base *>(ptr) != nullptr;
  }
}

#endif
