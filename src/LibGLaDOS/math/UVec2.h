#ifndef GLADOS_UVEC2_H
#define GLADOS_UVEC2_H

#include "Vec2.h"

namespace GLaDOS {
    class UVec2 {
      private:
        friend class Vec2;
        explicit UVec2(const Vec2& vec);
        Vec2 mVec;

      public:
        operator const Vec2&() const;  // should not be explicit
        const Vec2* operator->() const;
        const Vec2& operator*() const;
        ~UVec2() = default;

        static const UVec2 up, down, left, right, one, zero;
    };
}  // namespace GLaDOS

#endif  //GLADOS_UVEC2_H
