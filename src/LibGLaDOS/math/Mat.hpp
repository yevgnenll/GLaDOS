#ifndef GLADOS_MAT_H
#define GLADOS_MAT_H

#include <cstddef>
#include "utils/Stl.h"
#include "Vec.hpp"
#include "Math.h"

namespace GLaDOS {
    /*
     * GLaDOS's Mat<T, R, C> class represents Row major RxC matrix.
     * Row major means that consecutive memory layout places like below
     * T _m16[] = { a, b, c, d, e, f, g, h, i };
     *
     * Example of matrix representation in other Game Engines.
     * - Row major matrix -> DirectX, Unreal Engine
     * - Column major matrix -> OpenGL, Unity, glm
     */
    template <typename T, std::size_t R, std::size_t C>
    class Mat {
      public:
        Mat();
        Mat(T scalar);
        Mat(const Vec<T, R>& vector);
        Mat(const T(&scalars)[R*C]);
        Mat(const Vec<T, R>(&vectors)[C]);
        Mat(const Array<T, R*C>& scalars);
        Mat(const Array<Vec<T, R>, C>& vectors);
        Mat(Mat<T, R, C>&& other) noexcept;
        Mat<T, R, C>& operator=(Mat<T, R, C> other);  // copy and swap idiom (intentionally call by value)
        ~Mat() = default;

        template<typename = typename std::enable_if_t<R == C>>
        void makeIdentity();
        template<typename = typename std::enable_if_t<R == C>>
        bool isIdentity() const;
        Mat<T, R, C>& makeTranspose();
        template<typename = typename std::enable_if_t<R == C>>
        Mat<T, R, C>& makeInverse();
        T* pointer();

        Mat<T, R, C> operator+(const Mat<T, R, C>& other) const;
        Mat<T, R, C>& operator+=(const Mat<T, R, C>& other);
        Mat<T, R, C> operator-(const Mat<T, R, C>& other) const;
        Mat<T, R, C>& operator-=(const Mat<T, R, C>& other);
        template<std::size_t R2, std::size_t C2, typename = typename std::enable_if_t<C == R2>>
        Mat<T, R, C2> operator*(const Mat<T, R2, C2>& other) const;
        template<std::size_t R2, std::size_t C2, typename = typename std::enable_if_t<C == R2>>
        Mat<T, R, C2>& operator*=(const Mat<T, R2, C2>& other);
        bool operator==(const Mat<T, R, C>& other) const;
        bool operator!=(const Mat<T, R, C>& other) const;

        // Intentionally don't overload operator * function with vec4
        // because it makes matrix seem like column major matrix (M * v).

        Mat<T, R, C> operator*(const T& scalar) const;
        Mat<T, R, C>& operator*=(const T& scalar);
        Mat<T, R, C> operator/(const T& scalar) const;
        Mat<T, R, C>& operator/=(const T& scalar);

        static constexpr std::size_t dimension();
        static constexpr std::size_t size();
        template<typename = typename std::enable_if_t<R == C>>
        static constexpr Mat<T, R, C> identity();
        static constexpr Mat<T, R, C> zero();

        union {
            T _m44[R][C];
            T _m16[R*C];
            Vec<T, R> rows[C];
        };

      private:
        static void swap(Mat<T, R, C>& first, Mat<T, R, C>& second);
    };

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C>::Mat() {
        for (unsigned c = 0; c < C; c++) {
            for (unsigned r = 0; r < R; r++) {
                _m44[c][r] = Math::equal(c, r) ? T(1.0) : T(0.0);
            }
        }
    }

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C>::Mat(T scalar) {
        for (unsigned c = 0; c < C; c++) {
            for (unsigned r = 0; r < R; r++) {
                _m44[c][r] = scalar;
            }
        }
    }

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C>::Mat(const Vec<T, R>& vector) {
        for (unsigned c = 0; c < C; c++) {
            rows[c] = vector;
        }
    }

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C>::Mat(const T (&scalars)[R*C]) {
    }

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C>::Mat(const Vec<T, R> (&vectors)[C]) {
    }

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C>::Mat(const Array<GLaDOS::Vec<T, R>, C>& vectors) {
    }

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C>::Mat(Mat<T, R, C>&& other) noexcept {
    }

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C>& Mat<T, R, C>::operator=(Mat<T, R, C> other) {
        Mat<T, R, C>::swap(*this, other);
        return *this;
    }

    template <typename T, std::size_t R, std::size_t C>
    template <typename>
    void Mat<T, R, C>::makeIdentity() {
        for (unsigned c = 0; c < C; c++) {
            for (unsigned r = 0; r < R; r++) {
                _m44[c][r] = Math::equal(c, r) ? T(1.0) : T(0.0);
            }
        }
    }

    template <typename T, std::size_t R, std::size_t C>
    template <typename>
    bool Mat<T, R, C>::isIdentity() const {
        return *this == Mat<T, R, C>::identity();
    }

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C>& Mat<T, R, C>::makeTranspose() {
        return *this;
    }

    template <typename T, std::size_t R, std::size_t C>
    template <typename>
    Mat<T, R, C>& Mat<T, R, C>::makeInverse() {
        return *this;
    }

    template <typename T, std::size_t R, std::size_t C>
    T* Mat<T, R, C>::pointer() {
        return _m16;
    }

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C> Mat<T, R, C>::operator+(const Mat<T, R, C>& other) const {
        return Mat<T, R, C>();
    }

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C>& Mat<T, R, C>::operator+=(const Mat<T, R, C>& other) {
        return *this;
    }

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C> Mat<T, R, C>::operator-(const Mat<T, R, C>& other) const {
        return Mat<T, R, C>();
    }

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C>& Mat<T, R, C>::operator-=(const Mat<T, R, C>& other) {
        return *this;
    }

    template <typename T, std::size_t R, std::size_t C>
    template <std::size_t R2, std::size_t C2, typename>
    Mat<T, R, C2> Mat<T, R, C>::operator*(const Mat<T, R2, C2>& other) const {
        return Mat<T, R, C2>();
    }

    template <typename T, std::size_t R, std::size_t C>
    template <std::size_t R2, std::size_t C2, typename>
    Mat<T, R, C2>& Mat<T, R, C>::operator*=(const Mat<T, R2, C2>& other) {
        return *this;
    }

    template <typename T, std::size_t R, std::size_t C>
    bool Mat<T, R, C>::operator==(const Mat<T, R, C>& other) const {
        for (unsigned c = 0; c < C; c++) {
            for (unsigned r = 0; r < R; r++) {
                if (!Math::equal(_m44[c][r], other._m44[c][r])) {
                    return false;
                }
            }
        }
        return true;
    }

    template <typename T, std::size_t R, std::size_t C>
    bool Mat<T, R, C>::operator!=(const Mat<T, R, C>& other) const {
        return !(*this == other);  // use equal operator to implement not equal
    }

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C> Mat<T, R, C>::operator*(const T& scalar) const {
        return Mat<T, R, C>();
    }

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C>& Mat<T, R, C>::operator*=(const T& scalar) {
        return *this;
    }

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C> Mat<T, R, C>::operator/(const T& scalar) const {
        return Mat<T, R, C>();
    }

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C>& Mat<T, R, C>::operator/=(const T& scalar) {
        return *this;
    }

    template <typename T, std::size_t R, std::size_t C>
    constexpr std::size_t Mat<T, R, C>::dimension() {
        return R * C;
    }

    template <typename T, std::size_t R, std::size_t C>
    constexpr std::size_t Mat<T, R, C>::size() {
        return sizeof(T) * dimension();
    }

    template <typename T, std::size_t R, std::size_t C>
    template <typename>
    constexpr Mat<T, R, C> Mat<T, R, C>::identity() {
        Mat<T, R, C> mat;
        for (unsigned c = 0; c < C; c++) {
            for (unsigned r = 0; r < R; r++) {
                mat._m44[c][r] = Math::equal(c, r) ? T(1.0) : T(0.0);
            }
        }
        return mat;
    }

    template <typename T, std::size_t R, std::size_t C>
    constexpr Mat<T, R, C> Mat<T, R, C>::zero() {
        Mat<T, R, C> mat;
        for (unsigned c = 0; c < C; c++) {
            for (unsigned r = 0; r < R; r++) {
                mat._m44[c][r] = T(0.0);
            }
        }
        return mat;
    }

    template <typename T, std::size_t R, std::size_t C>
    void Mat<T, R, C>::swap(Mat<T, R, C>& first, Mat<T, R, C>& second) {
        using std::swap;

        for (int c = 0; c < C; c++) {
            for (int r = 0; r < R; r++) {
                swap(first._m44[c][r], second._m44[c][r]);
            }
        }
    }

    template <typename T>
    using Mat4x4 = Mat<T, 4, 4>;
    template <typename T>
    using Mat3x3 = Mat<T, 3, 3>;
    template <typename T>
    using Mat2x2 = Mat<T, 2, 2>;
}

#endif  // GLADOS_MAT_H
