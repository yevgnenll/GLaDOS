#include "UVec4.h"

namespace GameEngine {
  UVec4::UVec4(const Vec4& vec) : mVec{vec} {
  }

  UVec4::operator const Vec4&() const {
    return mVec;
  }

  const Vec4* UVec4::operator->() const {
    return &mVec;
  }

  const Vec4& UVec4::operator*() const {
    return mVec;
  }
}  // namespace GameEngine