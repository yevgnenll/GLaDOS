#ifndef GAMEENGINE_UTILITY_H
#define GAMEENGINE_UTILITY_H

#include "memory/Allocation.h"

namespace GameEngine {
  template <typename T>
  constexpr void deallocIterable(T&& pointer) {
    for (auto& i : pointer) {
      DELETE_T(i, T);
    }
    pointer.clear();
  }
}  // namespace GameEngine

#endif
