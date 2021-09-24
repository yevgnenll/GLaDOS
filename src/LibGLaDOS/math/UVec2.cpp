#include "UVec2.h"

namespace GLaDOS {
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

    const UVec2 UVec2::up = UVec2{Vec2{0.0, 1.0}};
    const UVec2 UVec2::down = UVec2{Vec2{0.0, -1.0}};
    const UVec2 UVec2::left = UVec2{Vec2{-1.0, 0.0}};
    const UVec2 UVec2::right = UVec2{Vec2{1.0, 0.0}};
    const UVec2 UVec2::one = UVec2{Vec2{1.0, 1.0}};
    const UVec2 UVec2::zero = UVec2{Vec2{0.0, 0.0}};
}  // namespace GLaDOS