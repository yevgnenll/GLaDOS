#ifndef GLADOS_VEC_H
#define GLADOS_VEC_H

#include "Math.h"
#include "VecSwizzle.hpp"

namespace GLaDOS {
    template <typename T, std::size_t N>
    class Vec {
      public:
        Vec();
        Vec(T scalar);
        ~Vec() = default;

        Vec(Vec<T, N>&& other) noexcept;
        Vec(const Vec<T, N>& other) = default;
        Vec<T, N>& operator=(Vec<T, N> other);  // copy and swap idiom

        bool operator==(const Vec<T, N>& other) const;
        bool operator!=(const Vec<T, N>& other) const;
        T& operator[](unsigned int index);
        const T& operator[](unsigned int index) const;
        Vec<T, N> operator+(const Vec<T, N>& other) const;
        Vec<T, N>& operator+=(const Vec<T, N>& other);
        Vec<T, N> operator-(const Vec<T, N>& other) const;
        Vec<T, N>& operator-=(const Vec<T, N>& other);
        Vec<T, N> operator*(const T& scalar) const;
        Vec<T, N>& operator*=(const T& scalar);
        Vec<T, N> operator/(const T& scalar) const;
        Vec<T, N>& operator/=(const T& scalar);

        Vec<T, N>& makeNegate();

        union {
            T v[N];
        };

      private:
        static void swap(Vec<T, N>& first, Vec<T, N>& second);
    };

    template <typename T, std::size_t N>
    Vec<T, N>::Vec() {
        for (unsigned int i = 0; i < N; i++) {
            v[i] = T(0);
        }
    }

    template <typename T, std::size_t N>
    Vec<T, N>::Vec(T scalar) {
        for (unsigned int i = 0; i < N; i++) {
            v[i] = scalar;
        }
    }

    template <typename T, std::size_t N>
    Vec<T, N>::Vec(Vec<T, N>&& other) noexcept {
        // copy and swap idiom (effective c++ section 11)
        Vec<T, N>::swap(*this, other);
    }

    template <typename T, std::size_t N>
    Vec<T, N>& Vec<T, N>::operator=(Vec<T, N> other) {
        // copy and swap idiom (effective c++ section 11)
        Vec<T, N>::swap(*this, other);
        return *this;
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

    template <typename T, std::size_t N>
    T& Vec<T, N>::operator[](unsigned int index) {
        return v[index];
    }

    template <typename T, std::size_t N>
    const T& Vec<T, N>::operator[](unsigned int index) const {
        return v[index];
    }

    template <typename T, std::size_t N>
    Vec<T, N> Vec<T, N>::operator+(const Vec<T, N>& other) const {
        // using op= (more effective c++ section 22)
        return Vec<T, N>(*this) += other;
    }

    template <typename T, std::size_t N>
    Vec<T, N>& Vec<T, N>::operator+=(const Vec<T, N>& other) {
        for (unsigned int i = 0; i < N; i++) {
            v[i] += other.v[i];
        }
        return *this;
    }

    template <typename T, std::size_t N>
    Vec<T, N> Vec<T, N>::operator-(const Vec<T, N>& other) const {
        // using op= (more effective c++ section 22)
        return Vec<T, N>(*this) -= other;
    }

    template <typename T, std::size_t N>
    Vec<T, N>& Vec<T, N>::operator-=(const Vec<T, N>& other) {
        for (unsigned int i = 0; i < N; i++) {
            v[i] -= other.v[i];
        }
        return *this;
    }

    template <typename T, std::size_t N>
    Vec<T, N> Vec<T, N>::operator*(const T& scalar) const {
        return Vec<T, N>(*this) *= scalar;
    }

    template <typename T, std::size_t N>
    Vec<T, N>& Vec<T, N>::operator*=(const T& scalar) {
        for (unsigned int i = 0; i < N; i++) {
            v[i] *= scalar;
        }
        return *this;
    }

    template <typename T, std::size_t N>
    Vec<T, N> Vec<T, N>::operator/(const T& scalar) const {
        return Vec<T, N>(*this) /= scalar;
    }

    template <typename T, std::size_t N>
    Vec<T, N>& Vec<T, N>::operator/=(const T& scalar) {
        for (unsigned int i = 0; i < N; i++) {
            v[i] /= scalar;
        }
        return *this;
    }

    template <typename T, std::size_t N>
    Vec<T, N>& Vec<T, N>::makeNegate() {
        for (unsigned int i = 0; i < N; i++) {
            v[i] = -v[i];
        }
        return *this;
    }

    template <typename T, std::size_t N>
    void Vec<T, N>::swap(Vec<T, N>& first, Vec<T, N>& second) {
        using std::swap;

        for (unsigned int i = 0; i < N; i++) {
            swap(first.v[i], second.v[i]);
        }
    }

    template <typename T>
    class Vec<T, 2> {
      public:
        Vec();
        Vec(const T& _x, const T& _y);
        ~Vec() = default;

        Vec(Vec<T, 2>&& other) noexcept;
        Vec(const Vec<T, 2>& other) = default;
        Vec<T, 2>& operator=(Vec<T, 2> other);  // copy and swap idiom

        bool operator==(const Vec<T, 2>& other) const;
        bool operator!=(const Vec<T, 2>& other) const;
        T& operator[](unsigned int index);
        const T& operator[](unsigned int index) const;
        Vec<T, 2> operator+(const Vec<T, 2>& other) const;
        Vec<T, 2>& operator+=(const Vec<T, 2>& other);
        Vec<T, 2> operator-(const Vec<T, 2>& other) const;
        Vec<T, 2>& operator-=(const Vec<T, 2>& other);
        Vec<T, 2> operator*(const T& scalar) const;
        Vec<T, 2>& operator*=(const T& scalar);
        Vec<T, 2> operator/(const T& scalar) const;
        Vec<T, 2>& operator/=(const T& scalar);

        union {
            T v[2];
            ScalarSwizzle<T, 0> x, r, s;
            ScalarSwizzle<T, 1> y, g, t;
            Vec2Swizzle<T, 0, 0> xx;
            Vec2Swizzle<T, 0, 1> xy;
            Vec2Swizzle<T, 1, 0> yx;
            Vec2Swizzle<T, 1, 1> yy;
        };

      private:
        static void swap(Vec<T, 2>& first, Vec<T, 2>& second);
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
    Vec<T, 2>::Vec(Vec<T, 2>&& other) noexcept {
        // copy and swap idiom (effective c++ section 11)
        Vec<T, 2>::swap(*this, other);
    }

    template <typename T>
    Vec<T, 2>& Vec<T, 2>::operator=(Vec<T, 2> other) {
        // copy and swap idiom (effective c++ section 11)
        Vec<T, 2>::swap(*this, other);
        return *this;
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
    T& Vec<T, 2>::operator[](unsigned int index) {
        return v[index];
    }

    template <typename T>
    const T& Vec<T, 2>::operator[](unsigned int index) const {
        return v[index];
    }

    template <typename T>
    Vec<T, 2> Vec<T, 2>::operator+(const Vec<T, 2>& other) const {
        // using op= (more effective c++ section 22)
        return Vec<T, 2>(*this) += other;
    }

    template <typename T>
    Vec<T, 2>& Vec<T, 2>::operator+=(const Vec<T, 2>& other) {
        v[0] += other.x;
        v[1] += other.y;
        return *this;
    }

    template <typename T>
    Vec<T, 2> Vec<T, 2>::operator-(const Vec<T, 2>& other) const {
        // using op= (more effective c++ section 22)
        return Vec<T, 2>(*this) -= other;
    }

    template <typename T>
    Vec<T, 2>& Vec<T, 2>::operator-=(const Vec<T, 2>& other) {
        v[0] -= other.x;
        v[1] -= other.y;
        return *this;
    }

    template <typename T>
    Vec<T, 2> Vec<T, 2>::operator*(const T& scalar) const {
        // using op= (more effective c++ section 22)
        return Vec<T, 2>(*this) *= scalar;
    }

    template <typename T>
    Vec<T, 2>& Vec<T, 2>::operator*=(const T& scalar) {
        v[0] *= scalar;
        v[1] *= scalar;
        return *this;
    }

    template <typename T>
    Vec<T, 2> Vec<T, 2>::operator/(const T& scalar) const {
        // using op= (more effective c++ section 22)
        return Vec<T, 2>(*this) /= scalar;
    }

    template <typename T>
    Vec<T, 2>& Vec<T, 2>::operator/=(const T& scalar) {
        v[0] /= scalar;
        v[1] /= scalar;
        return *this;
    }

    template <typename T>
    void Vec<T, 2>::swap(Vec<T, 2>& first, Vec<T, 2>& second) {
        using std::swap;

        for (unsigned int i = 0; i < 2; i++) {
            swap(first.v[i], second.v[i]);
        }
    }

    template <typename T>
    class Vec<T, 3> {
      public:
        Vec();
        Vec(const T& _x, const T& _y, const T& _z);
        ~Vec() = default;

        Vec(Vec<T, 3>&& other) noexcept;
        Vec(const Vec<T, 3>& other) = default;
        Vec<T, 3>& operator=(Vec<T, 3> other);  // copy and swap idiom

        bool operator==(const Vec<T, 3>& other) const;
        bool operator!=(const Vec<T, 3>& other) const;
        T& operator[](unsigned int index);
        const T& operator[](unsigned int index) const;
        Vec<T, 3> operator+(const Vec<T, 3>& other) const;
        Vec<T, 3>& operator+=(const Vec<T, 3>& other);
        Vec<T, 3> operator-(const Vec<T, 3>& other) const;
        Vec<T, 3>& operator-=(const Vec<T, 3>& other);
        Vec<T, 3> operator*(const T& scalar) const;
        Vec<T, 3>& operator*=(const T& scalar);
        Vec<T, 3> operator/(const T& scalar) const;
        Vec<T, 3>& operator/=(const T& scalar);

        union {
            T v[3];
            ScalarSwizzle<T, 0> x, r, s;
            ScalarSwizzle<T, 1> y, g, t;
            ScalarSwizzle<T, 2> z, b, p;

            Vec2Swizzle<T, 0, 0> xx;
            Vec2Swizzle<T, 0, 1> xy;
            Vec2Swizzle<T, 0, 2> xz;
            Vec2Swizzle<T, 1, 0> yx;
            Vec2Swizzle<T, 1, 1> yy;
            Vec2Swizzle<T, 1, 2> yz;
            Vec2Swizzle<T, 2, 0> zx;
            Vec2Swizzle<T, 2, 1> zy;
            Vec2Swizzle<T, 2, 2> zz;

            Vec3Swizzle<T, 0, 0, 0> xxx;
            Vec3Swizzle<T, 0, 0, 1> xxy;
            Vec3Swizzle<T, 0, 0, 2> xxz;
            Vec3Swizzle<T, 0, 1, 0> xyx;
            Vec3Swizzle<T, 0, 1, 1> xyy;
            Vec3Swizzle<T, 0, 1, 2> xyz;
            Vec3Swizzle<T, 0, 2, 0> xzx;
            Vec3Swizzle<T, 0, 2, 1> xzy;
            Vec3Swizzle<T, 0, 2, 2> xzz;

            Vec3Swizzle<T, 1, 0, 0> yxx;
            Vec3Swizzle<T, 1, 0, 1> yxy;
            Vec3Swizzle<T, 1, 0, 2> yxz;
            Vec3Swizzle<T, 1, 1, 0> yyx;
            Vec3Swizzle<T, 1, 1, 1> yyy;
            Vec3Swizzle<T, 1, 1, 2> yyz;
            Vec3Swizzle<T, 1, 2, 0> yzx;
            Vec3Swizzle<T, 1, 2, 1> yzy;
            Vec3Swizzle<T, 1, 2, 2> yzz;

            Vec3Swizzle<T, 2, 0, 0> zxx;
            Vec3Swizzle<T, 2, 0, 1> zxy;
            Vec3Swizzle<T, 2, 0, 2> zxz;
            Vec3Swizzle<T, 2, 1, 0> zyx;
            Vec3Swizzle<T, 2, 1, 1> zyy;
            Vec3Swizzle<T, 2, 1, 2> zyz;
            Vec3Swizzle<T, 2, 2, 0> zzx;
            Vec3Swizzle<T, 2, 2, 1> zzy;
            Vec3Swizzle<T, 2, 2, 2> zzz;
        };

      private:
        static void swap(Vec<T, 3>& first, Vec<T, 3>& second);
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
    Vec<T, 3>::Vec(Vec<T, 3>&& other) noexcept {
        // copy and swap idiom (effective c++ section 11)
        Vec<T, 3>::swap(*this, other);
    }

    template <typename T>
    Vec<T, 3>& Vec<T, 3>::operator=(Vec<T, 3> other) {
        // copy and swap idiom (effective c++ section 11)
        Vec<T, 3>::swap(*this, other);
        return *this;
    }

    template <typename T>
    bool Vec<T, 3>::operator==(const Vec<T, 3>& other) const {
        return (Math::equal(x, other.x) && Math::equal(y, other.y) && Math::equal(z, other.z));
    }

    template <typename T>
    bool Vec<T, 3>::operator!=(const Vec<T, 3>& other) const {
        return !(*this == other);
    }

    template <typename T>
    T& Vec<T, 3>::operator[](unsigned int index) {
        return v[index];
    }

    template <typename T>
    const T& Vec<T, 3>::operator[](unsigned int index) const {
        return v[index];
    }

    template <typename T>
    Vec<T, 3> Vec<T, 3>::operator+(const Vec<T, 3>& other) const {
        // using op= (more effective c++ section 22)
        return Vec<T, 3>(*this) += other;
    }

    template <typename T>
    Vec<T, 3>& Vec<T, 3>::operator+=(const Vec<T, 3>& other) {
        v[0] += other.x;
        v[1] += other.y;
        v[2] += other.z;
        return *this;
    }

    template <typename T>
    Vec<T, 3> Vec<T, 3>::operator-(const Vec<T, 3>& other) const {
        // using op= (more effective c++ section 22)
        return Vec<T, 3>(*this) -= other;
    }

    template <typename T>
    Vec<T, 3>& Vec<T, 3>::operator-=(const Vec<T, 3>& other) {
        v[0] -= other.x;
        v[1] -= other.y;
        v[2] -= other.z;
        return *this;
    }

    template <typename T>
    Vec<T, 3> Vec<T, 3>::operator*(const T& scalar) const {
        // using op= (more effective c++ section 22)
        return Vec<T, 3>(*this) *= scalar;
    }

    template <typename T>
    Vec<T, 3>& Vec<T, 3>::operator*=(const T& scalar) {
        v[0] *= scalar;
        v[1] *= scalar;
        v[2] *= scalar;
        return *this;
    }

    template <typename T>
    Vec<T, 3> Vec<T, 3>::operator/(const T& scalar) const {
        // using op= (more effective c++ section 22)
        return Vec<T, 3>(*this) /= scalar;
    }

    template <typename T>
    Vec<T, 3>& Vec<T, 3>::operator/=(const T& scalar) {
        v[0] /= scalar;
        v[1] /= scalar;
        v[2] /= scalar;
        return *this;
    }

    template <typename T>
    void Vec<T, 3>::swap(Vec<T, 3>& first, Vec<T, 3>& second) {
        using std::swap;

        for (unsigned int i = 0; i < 3; i++) {
            swap(first.v[i], second.v[i]);
        }
    }

    template <typename T>
    class Vec<T, 4> {
      public:
        Vec();
        Vec(const T& _x, const T& _y, const T& _z, const T& _w);
        ~Vec() = default;

        Vec(Vec<T, 4>&& other) noexcept;
        Vec(const Vec<T, 4>& other) = default;
        Vec<T, 4>& operator=(Vec<T, 4> other);  // copy and swap idiom

        bool operator==(const Vec<T, 4>& other) const;
        bool operator!=(const Vec<T, 4>& other) const;
        T& operator[](unsigned int index);
        const T& operator[](unsigned int index) const;
        Vec<T, 4> operator+(const Vec<T, 4>& other) const;
        Vec<T, 4>& operator+=(const Vec<T, 4>& other);
        Vec<T, 4> operator-(const Vec<T, 4>& other) const;
        Vec<T, 4>& operator-=(const Vec<T, 4>& other);
        Vec<T, 4> operator*(const T& scalar) const;
        Vec<T, 4>& operator*=(const T& scalar);
        Vec<T, 4> operator/(const T& scalar) const;
        Vec<T, 4>& operator/=(const T& scalar);

        union {
            T v[4];
            ScalarSwizzle<T, 0> x, r, s;
            ScalarSwizzle<T, 1> y, g, t;
            ScalarSwizzle<T, 2> z, b, p;
            ScalarSwizzle<T, 3> w, a, q;
        };

      private:
        static void swap(Vec<T, 4>& first, Vec<T, 4>& second);
    };

    template <typename T>
    Vec<T, 4>::Vec() {
        x = T(0);
        y = T(0);
        z = T(0);
        w = T(0);
    }

    template <typename T>
    Vec<T, 4>::Vec(const T& _x, const T& _y, const T& _z, const T& _w) {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }

    template <typename T>
    Vec<T, 4>::Vec(Vec<T, 4>&& other) noexcept {
        // copy and swap idiom (effective c++ section 11)
        Vec<T, 4>::swap(*this, other);
    }

    template <typename T>
    Vec<T, 4>& Vec<T, 4>::operator=(Vec<T, 4> other) {
        // copy and swap idiom (effective c++ section 11)
        Vec<T, 4>::swap(*this, other);
        return *this;
    }

    template <typename T>
    bool Vec<T, 4>::operator==(const Vec<T, 4>& other) const {
        return (Math::equal(x, other.x) && Math::equal(y, other.y) && Math::equal(z, other.z) && Math::equal(w, other.w));
    }

    template <typename T>
    bool Vec<T, 4>::operator!=(const Vec<T, 4>& other) const {
        return !(*this == other);
    }

    template <typename T>
    T& Vec<T, 4>::operator[](unsigned int index) {
        return v[index];
    }

    template <typename T>
    const T& Vec<T, 4>::operator[](unsigned int index) const {
        return v[index];
    }

    template <typename T>
    Vec<T, 4> Vec<T, 4>::operator+(const Vec<T, 4>& other) const {
        // using op= (more effective c++ section 22)
        return Vec<T, 4>(*this) += other;
    }

    template <typename T>
    Vec<T, 4>& Vec<T, 4>::operator+=(const Vec<T, 4>& other) {
        v[0] += other.x;
        v[1] += other.y;
        v[2] += other.z;
        v[3] += other.w;
        return *this;
    }

    template <typename T>
    Vec<T, 4> Vec<T, 4>::operator-(const Vec<T, 4>& other) const {
        // using op= (more effective c++ section 22)
        return Vec<T, 4>(*this) -= other;
    }

    template <typename T>
    Vec<T, 4>& Vec<T, 4>::operator-=(const Vec<T, 4>& other) {
        v[0] -= other.x;
        v[1] -= other.y;
        v[2] -= other.z;
        v[3] -= other.w;
        return *this;
    }

    template <typename T>
    Vec<T, 4> Vec<T, 4>::operator*(const T& scalar) const {
        // using op= (more effective c++ section 22)
        return Vec<T, 4>(*this) *= scalar;
    }

    template <typename T>
    Vec<T, 4>& Vec<T, 4>::operator*=(const T& scalar) {
        v[0] *= scalar;
        v[1] *= scalar;
        v[2] *= scalar;
        v[3] *= scalar;
        return *this;
    }

    template <typename T>
    Vec<T, 4> Vec<T, 4>::operator/(const T& scalar) const {
        // using op= (more effective c++ section 22)
        return Vec<T, 4>(*this) /= scalar;
    }

    template <typename T>
    Vec<T, 4>& Vec<T, 4>::operator/=(const T& scalar) {
        v[0] /= scalar;
        v[1] /= scalar;
        v[2] /= scalar;
        v[3] /= scalar;
        return *this;
    }

    template <typename T>
    void Vec<T, 4>::swap(Vec<T, 4>& first, Vec<T, 4>& second) {
        using std::swap;

        for (unsigned int i = 0; i < 4; i++) {
            swap(first.v[i], second.v[i]);
        }
    }

    template <typename T>
    using Vector2 = Vec<T, 2>;
    template <typename T>
    using Vector3 = Vec<T, 3>;
    template <typename T>
    using Vector4 = Vec<T, 4>;
}

#endif  // GLADOS_VEC_H
