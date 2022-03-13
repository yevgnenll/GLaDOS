#ifndef GLADOS_VECSWIZZLE_H
#define GLADOS_VECSWIZZLE_H

#include "Math.h"

namespace GLaDOS {
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

    template <typename VectorType, std::size_t A, std::size_t B>
    struct Vec2Swizzle {
        VectorType operator=(const VectorType& v) {
            return VectorType(d[A] = v.v[0], d[B] = v.v[1]);
        }
        operator VectorType() {
            return VectorType(d[A], d[B]);
        }
        bool operator==(const VectorType& other) const {
            return (Math::equal(d[A], other.v[0]) && Math::equal(d[B], other.v[1]));
        }
        bool operator!=(const VectorType& other) const {
            return !(*this == other);
        }
        typename VectorType::valueType d[2];
    };

    template <typename VectorType, std::size_t A, std::size_t B, std::size_t C>
    struct Vec3Swizzle {
        VectorType operator=(const VectorType& v) {
            return VectorType(d[A] = v.x, d[B] = v.y, d[C] = v.z);
        }
        operator VectorType() {
            return V(d[A], d[B], d[C]);
        }
        bool operator==(const VectorType& other) const {
            return (Math::equal(d[A], other.v[0]) && Math::equal(d[B], other.v[1]) && Math::equal(d[C], other.v[2]));
        }
        bool operator!=(const VectorType& other) const {
            return !(*this == other);
        }
        typename VectorType::valueType d[3];
    };

    template <typename VectorType, std::size_t A, std::size_t B, std::size_t C, std::size_t D>
    struct Vec4Swizzle {
        VectorType operator=(const VectorType& v) {
            return VectorType(d[A] = v.x, d[B] = v.y, d[C] = v.z, d[D] = v.w);
        }
        operator VectorType() {
            return V(d[A], d[B], d[C], d[D]);
        }
        bool operator==(const VectorType& other) const {
            return (Math::equal(d[A], other.v[0]) && Math::equal(d[B], other.v[1]) && Math::equal(d[C], other.v[2]) && Math::equal(d[D], other.v[3]));
        }
        bool operator!=(const VectorType& other) const {
            return !(*this == other);
        }
        typename VectorType::valueType d[4];
    };
}

#endif  // GLADOS_VECSWIZZLE_H
