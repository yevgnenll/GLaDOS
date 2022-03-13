#ifndef GLADOS_VECSWIZZLE_H
#define GLADOS_VECSWIZZLE_H

#include "Math.h"

namespace GLaDOS {
    template <typename T, std::size_t N>
    class Vec;

    template <typename T, std::size_t I>
    struct ScalarSwizzle {
        T& operator=(const T& x) {
            v[I] = x;
            return v[I];
        }
        operator T() const {
            return v[I];
        }
        T operator++(int) {
            return v[I]++;
        }
        T operator++() {
            return ++v[I];
        }
        T operator--(int) {
            return v[I]--;
        }
        T operator--() {
            return --v[I];
        }
        T v[1];
    };

    template <typename T, std::size_t A, std::size_t B>
    struct Vec2Swizzle {
        Vec<T, 2> operator=(const Vec<T, 2>& v) {
            return Vec<T, 2>(d[A] = v.x, d[B] = v.y);
        }
        operator Vec<T, 2>() {
            return Vec<T, 2>(d[A], d[B]);
        }
        bool operator==(const Vec<T, 2>& other) const {
            return (Math::equal(d[A], T(other.x)) && Math::equal(d[B], T(other.y)));
        }
        bool operator!=(const Vec<T, 2>& other) const {
            return !(*this == other);
        }
        T d[2];
    };

    template <typename T, std::size_t A, std::size_t B, std::size_t C>
    struct Vec3Swizzle {
        Vec<T, 3> operator=(const Vec<T, 3>& v) {
            return Vec<T, 3>(d[A] = v.x, d[B] = v.y, d[C] = v.z);
        }
        operator Vec<T, 3>() {
            return Vec<T, 3>(d[A], d[B], d[C]);
        }
        bool operator==(const Vec<T, 3>& other) const {
            return (Math::equal(d[A], T(other.x)) && Math::equal(d[B], T(other.y)) && Math::equal(d[C], T(other.z)));
        }
        bool operator!=(const Vec<T, 3>& other) const {
            return !(*this == other);
        }
        T d[3];
    };

    template <typename T, std::size_t A, std::size_t B, std::size_t C, std::size_t D>
    struct Vec4Swizzle {
        Vec<T, 4> operator=(const Vec<T, 4>& v) {
            return Vec<T, 4>(d[A] = v.x, d[B] = v.y, d[C] = v.z, d[D] = v.w);
        }
        operator Vec<T, 4>() {
            return Vec<T, 4>(d[A], d[B], d[C], d[D]);
        }
        bool operator==(const Vec<T, 4>& other) const {
            return (Math::equal(d[A], T(other.x)) && Math::equal(d[B], T(other.y)) && Math::equal(d[C], T(other.z)) && Math::equal(d[D], T(other.w)));
        }
        bool operator!=(const Vec<T, 4>& other) const {
            return !(*this == other);
        }
        T d[4];
    };
}

#endif  // GLADOS_VECSWIZZLE_H
