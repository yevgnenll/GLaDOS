#ifndef GAMEENGINE_UTILITY_H
#define GAMEENGINE_UTILITY_H

namespace GameEngine {

  template <typename T>
  constexpr void dealloc(T&& pointer) {
    if (pointer != nullptr) {
      delete pointer;
      pointer = nullptr;
    }
  }

  template <typename T>
  constexpr void deallocArray(T&& pointer) {
    if (pointer != nullptr) {
      delete[] pointer;
      pointer = nullptr;
    }
  }

  template <typename T>
  constexpr void deallocIterable(T&& pointer) {
    for (auto& i : pointer) {
      dealloc(i);
    }
    pointer.clear();
  }
}  // namespace GameEngine

#endif
