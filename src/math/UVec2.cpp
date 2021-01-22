#include "UVec2.h"

namespace GameEngine {
  UVec2::UVec2(const Vec2& vec) : mVec{vec} {
  }

  UVec2::operator const Vec2&() const {
    return mVec;
  }

  const Vec2* UVec2::operator->() const {
    return &mVec;
  }

  const Vec2& UVec2::operator*() const {
    return mVec;
  }
}  // namespace GameEngine