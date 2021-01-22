#ifndef GAMEENGINE_UVEC4_H
#define GAMEENGINE_UVEC4_H

#include "Vec4.h"

namespace GameEngine {
  class UVec4 {
  private:
    friend Vec4;
    explicit UVec4(const Vec4& vec);
    Vec4 mVec;

  public:
    operator const Vec4&() const;  // should not be explicit
    const Vec4* operator->() const;
    const Vec4& operator*() const;
  };
}  // namespace GameEngine

#endif  //GAMEENGINE_UVEC4_H
