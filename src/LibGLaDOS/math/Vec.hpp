#ifndef GLADOS_VEC_H
#define GLADOS_VEC_H

#include "utils/Stl.h"
#include "Math.h"
#include "VecSwizzle.hpp"
#include "UVec.hpp"

namespace GLaDOS {
    template <typename T, std::size_t R, std::size_t C>
    class Mat;
    template <typename T, std::size_t N>
    class Vec {
      public:
        Vec();
        explicit Vec(T scalar);
        template<typename... Ts, typename = std::enable_if_t<std::conjunction_v<std::is_same<T, Ts>...>>>
        Vec(Ts&&... scalars);
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
        template<std::size_t ROW, std::size_t COL, typename = typename std::enable_if_t<N == ROW>>
        Vec<T, COL> operator*(const Mat<T, ROW, COL>& matrix) const;

        real length() const;
        real squaredLength() const;
        real distance(const Vec<T, N>& other) const;
        real distanceSquare(const Vec<T, N>& other) const;

        Vec<T, N>& makeNegate();
        static Vec<T, N> negate(const Vec<T, N>& v);
        static T dot(const Vec<T, N>& a, const Vec<T, N>& b);
        static Vec<T, N> cross(const Vec<T, N>& a, const Vec<T, N>& b);
        static UVec<T, N> normalize(const Vec<T, N>& v);

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
    template<typename... Ts, typename>
    Vec<T, N>::Vec(Ts&&... scalars) {
        static_assert(sizeof...(scalars) == N, "Exceed vector dimension");
        Array<T, sizeof...(Ts)> arr = { std::forward<T>(scalars)... };
        for (unsigned int i = 0; i < N; i++) {
            v[i] = arr[i];
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
            if (!Math::equal(v[i], other.v[i])) {
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
    template<std::size_t ROW, std::size_t COL, typename>
    Vec<T, COL> Vec<T, N>::operator*(const Mat<T, ROW, COL>& matrix) const {
        Vec<T, COL> result;
        for (unsigned int col = 0; col < COL; col++) {
            T temp = T(0);
            for (unsigned int row = 0; row < ROW; row++) {
                temp += v[row] * matrix._mRC[row][col];
            }
            result[col] = temp;
        }
        return result;
    }

    template <typename T, std::size_t N>
    real Vec<T, N>::length() const {
        return Math::sqrt(squaredLength());
    }

    template <typename T, std::size_t N>
    real Vec<T, N>::squaredLength() const {
        return static_cast<real>(Vec<T, N>::dot(*this, *this));
    }

    template <typename T, std::size_t N>
    real Vec<T, N>::distance(const Vec<T, N>& other) const {
        return (*this - other).length();
    }

    template <typename T, std::size_t N>
    real Vec<T, N>::distanceSquare(const Vec<T, N>& other) const {
        return (*this - other).squaredLength();
    }

    template <typename T, std::size_t N>
    Vec<T, N>& Vec<T, N>::makeNegate() {
        for (unsigned int i = 0; i < N; i++) {
            v[i] = -v[i];
        }
        return *this;
    }

    template <typename T, std::size_t N>
    Vec<T, N> Vec<T, N>::negate(const Vec<T, N>& v) {
        Vec<T, N> result{v};
        return result.makeNegate();
    }

    template <typename T, std::size_t N>
    T Vec<T, N>::dot(const Vec<T, N>& a, const Vec<T, N>& b) {
        T result{T(0)};
        for (unsigned int i = 0; i < N; i++) {
            result += a.v[i] * b.v[i];
        }
        return result;
    }

    template <typename T, std::size_t N>
    Vec<T, N> Vec<T, N>::cross(const Vec<T, N>& a, const Vec<T, N>& b) {
        static_assert(N == 3, "only three dimension cross product is defined");
    }

    template <typename T, std::size_t N>
    UVec<T, N> Vec<T, N>::normalize(const Vec<T, N>& v) {
        real len = v.length();
        if (Math::zero(len) || Math::equal(len, real(1))) {
            return UVec<T, N>{v};
        }
        return UVec<T, N>{v / len};
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
        template<std::size_t ROW, std::size_t COL, typename = typename std::enable_if_t<2 == ROW>>
        Vec<T, COL> operator*(const Mat<T, ROW, COL>& matrix) const;

        real length() const;
        real squaredLength() const;
        real distance(const Vec<T, 2>& other) const;
        real distanceSquare(const Vec<T, 2>& other) const;

        Vec<T, 2>& makeNegate();
        static Vec<T, 2> negate(const Vec<T, 2>& v);
        static T dot(const Vec<T, 2>& a, const Vec<T, 2>& b);
        static UVec<T, 2> normalize(const Vec<T, 2>& v);

        union {
            T v[2];
            ScalarSwizzle<T, 0> x, r, s;
            ScalarSwizzle<T, 1> y, g, t;
            Vec2Swizzle<T, 0, 0> xx;
            Vec2Swizzle<T, 0, 1> xy;
            Vec2Swizzle<T, 1, 0> yx;
            Vec2Swizzle<T, 1, 1> yy;
        };
        static const Vec<T, 2> up, down, left, right, one, zero;

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
    template<std::size_t ROW, std::size_t COL, typename>
    Vec<T, COL> Vec<T, 2>::operator*(const Mat<T, ROW, COL>& matrix) const {
        Vec<T, COL> result;
        for (unsigned int col = 0; col < COL; col++) {
            T temp = T(0);
            for (unsigned int row = 0; row < 2; row++) {
                temp += v[row] * matrix._mRC[row][col];
            }
            result[col] = temp;
        }
        return result;
    }

    template <typename T>
    real Vec<T, 2>::length() const {
        return Math::sqrt(squaredLength());
    }

    template <typename T>
    real Vec<T, 2>::squaredLength() const {
        return static_cast<real>(Vec<T, 2>::dot(*this, *this));
    }

    template <typename T>
    real Vec<T, 2>::distance(const Vec<T, 2>& other) const {
        return (*this - other).length();
    }

    template <typename T>
    real Vec<T, 2>::distanceSquare(const Vec<T, 2>& other) const {
        return (*this - other).squaredLength();
    }

    template <typename T>
    Vec<T, 2>& Vec<T, 2>::makeNegate() {
        for (unsigned int i = 0; i < 2; i++) {
            v[i] = -v[i];
        }
        return *this;
    }

    template <typename T>
    Vec<T, 2> Vec<T, 2>::negate(const Vec<T, 2>& v) {
        Vec<T, 2> result{v};
        return result.makeNegate();
    }

    template <typename T>
    T Vec<T, 2>::dot(const Vec<T, 2>& a, const Vec<T, 2>& b) {
        return a.x * b.x + a.y * b.y;
    }

    template <typename T>
    UVec<T, 2> Vec<T, 2>::normalize(const Vec<T, 2>& v) {
        real len = v.length();
        if (Math::zero(len) || Math::equal(len, real(1))) {
            return UVec<T, 2>{v};
        }
        return UVec<T, 2>{v / len};
    }

    template <typename T>
    void Vec<T, 2>::swap(Vec<T, 2>& first, Vec<T, 2>& second) {
        using std::swap;

        for (unsigned int i = 0; i < 2; i++) {
            swap(first.v[i], second.v[i]);
        }
    }

    template <typename T>
    const Vec<T, 2> Vec<T, 2>::up = Vec<T, 2>{T(0), T(1)};
    template <typename T>
    const Vec<T, 2> Vec<T, 2>::down = Vec<T, 2>{T(0), T(-1)};
    template <typename T>
    const Vec<T, 2> Vec<T, 2>::left = Vec<T, 2>{T(-1), T(0)};
    template <typename T>
    const Vec<T, 2> Vec<T, 2>::right = Vec<T, 2>{T(1), T(0)};
    template <typename T>
    const Vec<T, 2> Vec<T, 2>::one = Vec<T, 2>{T(1), T(1)};
    template <typename T>
    const Vec<T, 2> Vec<T, 2>::zero = Vec<T, 2>{T(0), T(0)};

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
        template<std::size_t ROW, std::size_t COL, typename = typename std::enable_if_t<3 == ROW>>
        Vec<T, COL> operator*(const Mat<T, ROW, COL>& matrix) const;

        real length() const;
        real squaredLength() const;
        real distance(const Vec<T, 3>& other) const;
        real distanceSquare(const Vec<T, 3>& other) const;

        Vec<T, 3>& makeNegate();
        static Vec<T, 3> negate(const Vec<T, 3>& v);
        static T dot(const Vec<T, 3>& a, const Vec<T, 3>& b);
        static Vec<T, 3> cross(const Vec<T, 3>& a, const Vec<T, 3>& b);
        static UVec<T, 3> normalize(const Vec<T, 3>& v);

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
        static const Vec<T, 3> up, down, left, right, forward, backward, one, zero;

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
    template<std::size_t ROW, std::size_t COL, typename>
    Vec<T, COL> Vec<T, 3>::operator*(const Mat<T, ROW, COL>& matrix) const {
        Vec<T, COL> result;
        for (unsigned int col = 0; col < COL; col++) {
            T temp = T(0);
            for (unsigned int row = 0; row < 3; row++) {
                temp += v[row] * matrix._mRC[row][col];
            }
            result[col] = temp;
        }
        return result;
    }

    template <typename T>
    real Vec<T, 3>::length() const {
        return Math::sqrt(squaredLength());
    }

    template <typename T>
    real Vec<T, 3>::squaredLength() const {
        return static_cast<real>(Vec<T, 3>::dot(*this, *this));
    }

    template <typename T>
    real Vec<T, 3>::distance(const Vec<T, 3>& other) const {
        return (*this - other).length();
    }

    template <typename T>
    real Vec<T, 3>::distanceSquare(const Vec<T, 3>& other) const {
        return (*this - other).squaredLength();
    }

    template <typename T>
    Vec<T, 3>& Vec<T, 3>::makeNegate() {
        for (unsigned int i = 0; i < 3; i++) {
            v[i] = -v[i];
        }
        return *this;
    }

    template <typename T>
    Vec<T, 3> Vec<T, 3>::negate(const Vec<T, 3>& v) {
        Vec<T, 3> result{v};
        return result.makeNegate();
    }

    template <typename T>
    T Vec<T, 3>::dot(const Vec<T, 3>& a, const Vec<T, 3>& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    template <typename T>
    Vec<T, 3> Vec<T, 3>::cross(const Vec<T, 3>& a, const Vec<T, 3>& b) {
        return Vec<T, 3>{a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
    }

    template <typename T>
    UVec<T, 3> Vec<T, 3>::normalize(const Vec<T, 3>& v) {
        real len = v.length();
        if (Math::zero(len) || Math::equal(len, real(1))) {
            return UVec<T, 3>{v};
        }
        return UVec<T, 3>{v / len};
    }

    template <typename T>
    void Vec<T, 3>::swap(Vec<T, 3>& first, Vec<T, 3>& second) {
        using std::swap;

        for (unsigned int i = 0; i < 3; i++) {
            swap(first.v[i], second.v[i]);
        }
    }

    template <typename T>
    const Vec<T, 3> Vec<T, 3>::up = Vec<T, 3>{T(0), T(1), T(0)};
    template <typename T>
    const Vec<T, 3> Vec<T, 3>::down = Vec<T, 3>{T(0), T(-1), T(0)};
    template <typename T>
    const Vec<T, 3> Vec<T, 3>::left = Vec<T, 3>{T(-1), T(0), T(0)};
    template <typename T>
    const Vec<T, 3> Vec<T, 3>::right = Vec<T, 3>{T(1), T(0), T(0)};
    template <typename T>
    const Vec<T, 3> Vec<T, 3>::forward = Vec<T, 3>{T(0), T(0), T(-1)};
    template <typename T>
    const Vec<T, 3> Vec<T, 3>::backward = Vec<T, 3>{T(0), T(0), T(1)};
    template <typename T>
    const Vec<T, 3> Vec<T, 3>::one = Vec<T, 3>{T(1), T(1), T(1)};
    template <typename T>
    const Vec<T, 3> Vec<T, 3>::zero = Vec<T, 3>{T(0), T(0), T(0)};

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
        template<std::size_t ROW, std::size_t COL, typename = typename std::enable_if_t<4 == ROW>>
        Vec<T, COL> operator*(const Mat<T, ROW, COL>& matrix) const;

        real length() const;
        real squaredLength() const;
        real distance(const Vec<T, 4>& other) const;
        real distanceSquare(const Vec<T, 4>& other) const;

        Vec<T, 4>& makeNegate();
        static Vec<T, 4> negate(const Vec<T, 4>& v);
        static T dot(const Vec<T, 4>& a, const Vec<T, 4>& b);
        static UVec<T, 4> normalize(const Vec<T, 4>& v);

        union {
            T v[4];
            ScalarSwizzle<T, 0> x, r, s;
            ScalarSwizzle<T, 1> y, g, t;
            ScalarSwizzle<T, 2> z, b, p;
            ScalarSwizzle<T, 3> w, a, q;
        };
        static const Vec<T, 4> up, down, left, right, forward, backward, one, zero;

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
    template<std::size_t ROW, std::size_t COL, typename>
    Vec<T, COL> Vec<T, 4>::operator*(const Mat<T, ROW, COL>& matrix) const {
        Vec<T, COL> result;
        for (unsigned int col = 0; col < COL; col++) {
            T temp = T(0);
            for (unsigned int row = 0; row < 4; row++) {
                temp += v[row] * matrix._mRC[row][col];
            }
            result[col] = temp;
        }
        return result;
    }

    template <typename T>
    real Vec<T, 4>::length() const {
        return Math::sqrt(squaredLength());
    }

    template <typename T>
    real Vec<T, 4>::squaredLength() const {
        return static_cast<real>(Vec<T, 4>::dot(*this, *this));
    }

    template <typename T>
    real Vec<T, 4>::distance(const Vec<T, 4>& other) const {
        return (*this - other).length();
    }

    template <typename T>
    real Vec<T, 4>::distanceSquare(const Vec<T, 4>& other) const {
        return (*this - other).squaredLength();
    }

    template <typename T>
    Vec<T, 4>& Vec<T, 4>::makeNegate() {
        for (unsigned int i = 0; i < 4; i++) {
            v[i] = -v[i];
        }
        return *this;
    }

    template <typename T>
    Vec<T, 4> Vec<T, 4>::negate(const Vec<T, 4>& v) {
        Vec<T, 4> result{v};
        return result.makeNegate();
    }

    template <typename T>
    T Vec<T, 4>::dot(const Vec<T, 4>& a, const Vec<T, 4>& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }

    template <typename T>
    UVec<T, 4> Vec<T, 4>::normalize(const Vec<T, 4>& v) {
        real len = v.length();
        if (Math::zero(len) || Math::equal(len, real(1))) {
            return UVec<T, 4>{v};
        }
        return UVec<T, 4>{v / len};
    }

    template <typename T>
    void Vec<T, 4>::swap(Vec<T, 4>& first, Vec<T, 4>& second) {
        using std::swap;

        for (unsigned int i = 0; i < 4; i++) {
            swap(first.v[i], second.v[i]);
        }
    }

    template <typename T>
    const Vec<T, 4> Vec<T, 4>::up = Vec<T, 4>{T(0), T(1), T(0), T(0)};
    template <typename T>
    const Vec<T, 4> Vec<T, 4>::down = Vec<T, 4>{T(0), T(-1), T(0), T(0)};
    template <typename T>
    const Vec<T, 4> Vec<T, 4>::left = Vec<T, 4>{T(-1), T(0), T(0), T(0)};
    template <typename T>
    const Vec<T, 4> Vec<T, 4>::right = Vec<T, 4>{T(1), T(0), T(0), T(0)};
    template <typename T>
    const Vec<T, 4> Vec<T, 4>::forward = Vec<T, 4>{T(0), T(0), T(-1), T(0)};
    template <typename T>
    const Vec<T, 4> Vec<T, 4>::backward = Vec<T, 4>{T(0), T(0), T(1), T(0)};
    template <typename T>
    const Vec<T, 4> Vec<T, 4>::one = Vec<T, 4>{T(1), T(1), T(1), T(1)};
    template <typename T>
    const Vec<T, 4> Vec<T, 4>::zero = Vec<T, 4>{T(0), T(0), T(0), T(0)};

    template <typename T>
    using Vector2 = Vec<T, 2>;
    template <typename T>
    using Vector3 = Vec<T, 3>;
    template <typename T>
    using Vector4 = Vec<T, 4>;
}

#endif  // GLADOS_VEC_H
