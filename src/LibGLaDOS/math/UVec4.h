#ifndef GLADOS_UVEC4_H
#define GLADOS_UVEC4_H

#include "Vec4.h"

namespace GLaDOS {
  class UVec4 {
  private:
    friend class Vec4;
    explicit UVec4(const Vec4& vec);
    Vec4 mVec;

  public:
    operator const Vec4&() const;  // should not be explicit
    const Vec4* operator->() const;
    const Vec4& operator*() const;
    ~UVec4() = default;
  };
}  // namespace GLaDOS

#endif  //GLADOS_UVEC4_H
