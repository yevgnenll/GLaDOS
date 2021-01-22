#ifndef GAMEENGINE_UVEC2_H
#define GAMEENGINE_UVEC2_H

#include "Vec2.h"

namespace GameEngine {
  class UVec2 {
  private:
    friend Vec2;
    explicit UVec2(const Vec2& vec);
    Vec2 mVec;

  public:
    operator const Vec2&() const;  // should not be explicit
    const Vec2* operator->() const;
    const Vec2& operator*() const;
  };
}  // namespace GameEngine

#endif  //GAMEENGINE_UVEC2_H
