#ifndef GLADOS_UVEC3_H
#define GLADOS_UVEC3_H

#include "Vec3.h"

namespace GLaDOS {
  class UVec3 {
  private:
    friend Vec3;
    explicit UVec3(const Vec3& vec);
    Vec3 mVec;

  public:
    operator const Vec3&() const;  // should not be explicit
    const Vec3* operator->() const;
    const Vec3& operator*() const;
  };
}  // namespace GLaDOS

#endif  //GLADOS_UVEC3_H
