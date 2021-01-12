#ifndef GAMEENGINE_MATH_H
#define GAMEENGINE_MATH_H

namespace GameEngine {
  class Math {
  public:
    Math() = delete;

    static float PingPong(float t, float len);
  };
}  // namespace GameEngine

#endif
