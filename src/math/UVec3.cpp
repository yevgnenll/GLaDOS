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

  const UVec3 UVec3::up = UVec3{Vec3{0.0, 1.0, 0.0}};
  const UVec3 UVec3::down = UVec3{Vec3{0.0, -1.0, 0.0}};
  const UVec3 UVec3::left = UVec3{Vec3{-1.0, 0.0, 0.0}};
  const UVec3 UVec3::right = UVec3{Vec3{1.0, 0.0, 0.0}};
  const UVec3 UVec3::forward = UVec3{Vec3{0.0, 0.0, -1.0}};
  const UVec3 UVec3::backward = UVec3{Vec3{0.0, 0.0, 1.0}};
  const UVec3 UVec3::one = UVec3{Vec3{1.0, 1.0, 1.0}};
  const UVec3 UVec3::zero = UVec3{Vec3{0.0, 0.0, 0.0}};
}  // namespace GLaDOS