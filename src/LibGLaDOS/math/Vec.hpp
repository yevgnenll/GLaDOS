#ifndef GLADOS_VEC_H
#define GLADOS_VEC_H

#include "Math.h"
#include "VecSwizzle.hpp"

namespace GLaDOS {
    template <typename T, std::size_t N>
    class Vec {
      public:
        typedef T valueType;

        Vec();
        ~Vec() = default;

        bool operator==(const Vec<T, N>& other) const;
        bool operator!=(const Vec<T, N>& other) const;

        union {
            T v[N];
        };
    };

    template <typename T, std::size_t N>
    Vec<T, N>::Vec() {
        for (unsigned int i = 0; i < N; i++) {
            v[i] = T(0);
        }
    }

    template <typename T, std::size_t N>
    bool Vec<T, N>::operator==(const Vec<T, N>& other) const {
        for (unsigned int i = 0; i < N; i++) {
            if (Math::equal(v[i], other.v[i])) {
                return false;
            }
        }
        return true;
    }

    template <typename T, std::size_t N>
    bool Vec<T, N>::operator!=(const Vec<T, N>& other) const {
        return !(*this == other);
    }

    template <typename T>
    class Vec<T, 2> {
      public:
        typedef T valueType;

        Vec();
        Vec(const T& _x, const T& _y);
        ~Vec() = default;

        bool operator==(const Vec<T, 2>& other) const;
        bool operator!=(const Vec<T, 2>& other) const;

        union {
            T v[2];
            ScalarSwizzle<T, 0> x, r, s;
            ScalarSwizzle<T, 1> y, g, t;
            Vec2Swizzle<Vec<T, 2>, 0, 0> xx;
            Vec2Swizzle<Vec<T, 2>, 0, 1> xy;
            Vec2Swizzle<Vec<T, 2>, 1, 0> yx;
            Vec2Swizzle<Vec<T, 2>, 1, 1> yy;
        };
    };

    template <typename T>
    Vec<T, 2>::Vec() {
        x = T(0);
        y = T(0);
    }

    template <typename T>
    Vec<T, 2>::Vec(const T& _x, const T& _y){
        x = _x;
        y = _y;
    }

    template <typename T>
    bool Vec<T, 2>::operator==(const Vec<T, 2>& other) const {
        return (Math::equal(x, other.x) && Math::equal(y, other.y));
    }

    template <typename T>
    bool Vec<T, 2>::operator!=(const Vec<T, 2>& other) const {
        return !(*this == other);
    }

    template <typename T>
    class Vec<T, 3> {
      private:
      public:
        typedef T valueType;

        Vec();
        Vec(const T& _x, const T& _y, const T& _z);
        ~Vec() = default;

        bool operator==(const Vec<T, 3>& other) const;
        bool operator!=(const Vec<T, 3>& other) const;

        union {
            T v[3];
            ScalarSwizzle<T, 0> x, r, s;
            ScalarSwizzle<T, 1> y, g, t;
            ScalarSwizzle<T, 2> z, b, p;

//            Vec2Swizzle<Vec<T, 2>, 0, 0> xx;
//            Vec2Swizzle<Vec<T, 2>, 0, 1> xy;
            Vec2Swizzle<Vec<T, 2>, 0, 2> xz;
//            Vec2Swizzle<Vec<T, 2>, 1, 0> yx;
//            Vec2Swizzle<Vec<T, 2>, 1, 1> yy;
            Vec2Swizzle<Vec<T, 2>, 1, 2> yz;
            Vec2Swizzle<Vec<T, 2>, 2, 0> zx;
            Vec2Swizzle<Vec<T, 2>, 2, 1> zy;
            Vec2Swizzle<Vec<T, 2>, 2, 2> zz;

            Vec3Swizzle<Vec<T, 3>, 0, 0, 0> xxx;
            Vec3Swizzle<Vec<T, 3>, 0, 0, 1> xxy;
            Vec3Swizzle<Vec<T, 3>, 0, 0, 2> xxz;
            Vec3Swizzle<Vec<T, 3>, 0, 1, 0> xyx;
            Vec3Swizzle<Vec<T, 3>, 0, 1, 1> xyy;
            Vec3Swizzle<Vec<T, 3>, 0, 1, 2> xyz;
            Vec3Swizzle<Vec<T, 3>, 0, 2, 0> xzx;
            Vec3Swizzle<Vec<T, 3>, 0, 2, 1> xzy;
            Vec3Swizzle<Vec<T, 3>, 0, 2, 2> xzz;

            Vec3Swizzle<Vec<T, 3>, 1, 0, 0> yxx;
            Vec3Swizzle<Vec<T, 3>, 1, 0, 1> yxy;
            Vec3Swizzle<Vec<T, 3>, 1, 0, 2> yxz;
            Vec3Swizzle<Vec<T, 3>, 1, 1, 0> yyx;
            Vec3Swizzle<Vec<T, 3>, 1, 1, 1> yyy;
            Vec3Swizzle<Vec<T, 3>, 1, 1, 2> yyz;
            Vec3Swizzle<Vec<T, 3>, 1, 2, 0> yzx;
            Vec3Swizzle<Vec<T, 3>, 1, 2, 1> yzy;
            Vec3Swizzle<Vec<T, 3>, 1, 2, 2> yzz;

            Vec3Swizzle<Vec<T, 3>, 2, 0, 0> zxx;
            Vec3Swizzle<Vec<T, 3>, 2, 0, 1> zxy;
            Vec3Swizzle<Vec<T, 3>, 2, 0, 2> zxz;
            Vec3Swizzle<Vec<T, 3>, 2, 1, 0> zyx;
            Vec3Swizzle<Vec<T, 3>, 2, 1, 1> zyy;
            Vec3Swizzle<Vec<T, 3>, 2, 1, 2> zyz;
            Vec3Swizzle<Vec<T, 3>, 2, 2, 0> zzx;
            Vec3Swizzle<Vec<T, 3>, 2, 2, 1> zzy;
            Vec3Swizzle<Vec<T, 3>, 2, 2, 2> zzz;
        };
    };

    template <typename T>
    Vec<T, 3>::Vec() {
        x = T(0);
        y = T(0);
        z = T(0);
    }

    template <typename T>
    Vec<T, 3>::Vec(const T& _x, const T& _y, const T& _z) {
        x = _x;
        y = _y;
        z = _z;
    }

    template <typename T>
    bool Vec<T, 3>::operator==(const Vec<T, 3>& other) const {
        return (Math::equal(x, other.x) && Math::equal(y, other.y) && Math::equal(z, other.z));
    }

    template <typename T>
    bool Vec<T, 3>::operator!=(const Vec<T, 3>& other) const {
        return !(*this == other);
    }

//    template <typename T>
//    class Vec<T, 4> {
//      private:
//      public:
//    };

    template <typename T>
    using Vector2 = Vec<T, 2>;
    template <typename T>
    using Vector3 = Vec<T, 3>;
    template <typename T>
    using Vector4 = Vec<T, 4>;
}

#endif  // GLADOS_VEC_H
