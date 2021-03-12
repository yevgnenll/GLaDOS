#ifndef GLADOS_UVEC3_H
#define GLADOS_UVEC3_H

#include "Vec3.h"

namespace GLaDOS {
  class UVec3 {
  private:
    friend class Vec3;
    explicit UVec3(const Vec3& vec);
    Vec3 mVec;

  public:
    operator const Vec3&() const;  // should not be explicit
    const Vec3* operator->() const;
    const Vec3& operator*() const;
    ~UVec3() = default;

    static const UVec3 up, down, left, right, forward, backward, one, zero;
  };
}  // namespace GLaDOS

#endif  //GLADOS_UVEC3_H
