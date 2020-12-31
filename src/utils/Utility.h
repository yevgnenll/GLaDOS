#ifndef GAMEENGINE_UTILITY_H
#define GAMEENGINE_UTILITY_H

#include "memory/Allocation.h"
#include "utils/Debug.h"

namespace GameEngine {
#define LOG_TRACE(fmt, ...) GameEngine::Debug::getInstance()->info({__FILE__, __LINE__, __FUNCTION__}, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) GameEngine::Debug::getInstance()->info({__FILE__, __LINE__, __FUNCTION__}, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) GameEngine::Debug::getInstance()->info({__FILE__, __LINE__, __FUNCTION__}, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) GameEngine::Debug::getInstance()->warn({__FILE__, __LINE__, __FUNCTION__}, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) GameEngine::Debug::getInstance()->error({__FILE__, __LINE__, __FUNCTION__}, fmt, ##__VA_ARGS__)

  template <typename T>
  constexpr void deallocIterable(T&& pointer) {
    for (auto& i : pointer) {
      DELETE_T(i, T);
    }
    pointer.clear();
  }
}  // namespace GameEngine

#endif
