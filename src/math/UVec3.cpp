#include "UVec3.h"

namespace GLaDOS {
  UVec3::UVec3(const Vec3& vec) : mVec{vec} {
  }

  UVec3::operator const Vec3&() const {
    return mVec;
  }

  const Vec3* UVec3::operator->() const {
    return &mVec;
  }

  const Vec3& UVec3::operator*() const {
    return mVec;
  }
}  // namespace GLaDOS