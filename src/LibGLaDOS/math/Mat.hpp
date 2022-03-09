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
        template<typename... Ts, typename = std::enable_if_t<std::conjunction_v<std::is_same<T, Ts>...>>>
        Mat(Ts&&... scalars);
        Mat(const Mat<T, R, C>& other);
        Mat(Mat<T, R, C>&& other) noexcept;
        Mat<T, R, C>& operator=(Mat<T, R, C> other);  // copy and swap idiom (intentionally call by value)
        ~Mat() = default;

        template<std::size_t ROW = R, std::size_t COL = C, typename = typename std::enable_if_t<ROW == COL>>
        void makeIdentity();
        template<std::size_t ROW = R, std::size_t COL = C, typename = typename std::enable_if_t<ROW == COL>>
        bool isIdentity() const;
        Mat<T, R, C>& makeTranspose();
        template<std::size_t ROW = R, std::size_t COL = C, typename = typename std::enable_if_t<ROW == COL>>
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

        static Mat<T, R, C> from(const T& scalar);
        static Mat<T, R, C> from(const Vec<T, R>& vector);
        static constexpr std::size_t dimension();
        static constexpr std::size_t size();
        template<std::size_t ROW = R, std::size_t COL = C, typename = typename std::enable_if_t<ROW == COL>>
        static constexpr Mat<T, ROW, COL> identity();
        static constexpr Mat<T, R, C> zero();
        static Vec<T, C> diagonal(const Mat<T, R, C>& other);
        static Mat<T, R, C> transpose(const Mat<T, R, C>& other);
        static T minor(const Mat<T, R, C>& other, std::size_t row, std::size_t col);
        static T cofactor(const Mat<T, R, C>& other, std::size_t row, std::size_t col);
        static T determinant(const Mat<T, R, C>& other);
        static Mat<T, R, C> adjugate(const Mat<T, R, C>& other);
        template<std::size_t ROW = R, std::size_t COL = C, typename = typename std::enable_if_t<ROW == COL>>
        static Mat<T, ROW, COL> inverse(const Mat<T, ROW, COL>& other);
        static T inverseDeterminant(const Mat<T, R, C>& other);
        static T trace(const Mat<T, R, C>& other);

        union {
            T _m44[R][C];
            T _m16[R*C];
            Vec<T, C> rows[R];
        };

      private:
        static void swap(Mat<T, R, C>& first, Mat<T, R, C>& second);
    };

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C>::Mat() {
        for (unsigned int r = 0; r < R; r++) {
            for (unsigned int c = 0; c < C; c++) {
                if constexpr (R == C) {
                    _m44[r][c] = Math::equal(r, c) ? T(1.0) : T(0.0);
                } else {
                    _m44[r][c] = T(0.0);
                }
            }
        }
    }

    template <typename T, std::size_t R, std::size_t C>
    template <typename... Ts, typename>
    Mat<T, R, C>::Mat(Ts&&... scalars) {
        static_assert(sizeof...(scalars) == (R * C), "Exceed matrix dimension");
        Array<T, sizeof...(Ts)> arr = { std::forward<T>(scalars)... };
        for (unsigned int r = 0; r < R; r++) {
            for (unsigned int c = 0; c < C; c++) {
                _m44[r][c] = arr[r * C + c];
            }
        }
    }

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C>::Mat(const Mat<T, R, C>& other) {
        for (unsigned int r = 0; r < R; r++) {
            for (unsigned int c = 0; c < C; c++) {
                _m44[r][c] = other._m44[r][c];
            }
        }
    }

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C>::Mat(Mat<T, R, C>&& other) noexcept {
        Mat<T, R, C>::swap(*this, other);
    }

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C>& Mat<T, R, C>::operator=(Mat<T, R, C> other) {
        Mat<T, R, C>::swap(*this, other);
        return *this;
    }

    template <typename T, std::size_t R, std::size_t C>
    template <std::size_t ROW, std::size_t COL, typename>
    void Mat<T, R, C>::makeIdentity() {
        for (unsigned int r = 0; r < ROW; r++) {
            for (unsigned int c = 0; c < COL; c++) {
                _m44[r][c] = Math::equal(r, c) ? T(1.0) : T(0.0);
            }
        }
    }

    template <typename T, std::size_t R, std::size_t C>
    template <std::size_t ROW, std::size_t COL, typename>
    bool Mat<T, R, C>::isIdentity() const {
        return *this == Mat<T, ROW, COL>::identity();
    }

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C>& Mat<T, R, C>::makeTranspose() {
        return *this;
    }

    template <typename T, std::size_t R, std::size_t C>
    template <std::size_t ROW, std::size_t COL, typename>
    Mat<T, R, C>& Mat<T, R, C>::makeInverse() {
        return *this;
    }

    template <typename T, std::size_t R, std::size_t C>
    T* Mat<T, R, C>::pointer() {
        return _m16;
    }

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C> Mat<T, R, C>::operator+(const Mat<T, R, C>& other) const {
        return Mat<T, R, C>(*this) += other;
    }

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C>& Mat<T, R, C>::operator+=(const Mat<T, R, C>& other) {
        for (unsigned int r = 0; r < R; r++) {
            for (unsigned int c = 0; c < C; c++) {
                _m44[r][c] += other._m44[r][c];
            }
        }
        return *this;
    }

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C> Mat<T, R, C>::operator-(const Mat<T, R, C>& other) const {
        return Mat<T, R, C>(*this) -= other;
    }

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C>& Mat<T, R, C>::operator-=(const Mat<T, R, C>& other) {
        for (unsigned int r = 0; r < R; r++) {
            for (unsigned int c = 0; c < C; c++) {
                _m44[r][c] -= other._m44[r][c];
            }
        }
        return *this;
    }

    template <typename T, std::size_t R, std::size_t C>
    template <std::size_t R2, std::size_t C2, typename>
    Mat<T, R, C2> Mat<T, R, C>::operator*(const Mat<T, R2, C2>& other) const {
        return Mat<T, R, C>(*this) *= other;
    }

    template <typename T, std::size_t R, std::size_t C>
    template <std::size_t R2, std::size_t C2, typename>
    Mat<T, R, C2>& Mat<T, R, C>::operator*=(const Mat<T, R2, C2>& other) {
        Mat<T, R, C2> result;
        for (unsigned int r = 0; r < R; r++) {
            for (unsigned int c = 0; c < C2; c++) {
                result._m44[r][c] = T(0);
                for (unsigned int k = 0; k < C; k++) {
                    result._m44[r][c] += _m44[r][k] * other._m44[k][c];
                }
            }
        }
        return *this = result;
    }

    template <typename T, std::size_t R, std::size_t C>
    bool Mat<T, R, C>::operator==(const Mat<T, R, C>& other) const {
        for (unsigned int r = 0; r < R; r++) {
            for (unsigned int c = 0; c < C; c++) {
                if (!Math::equal(_m44[r][c], other._m44[r][c])) {
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
        return Mat<T, R, C>(*this) *= scalar;
    }

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C>& Mat<T, R, C>::operator*=(const T& scalar) {
        for (auto& element : _m16) {
            element *= scalar;
        }
        return *this;
    }

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C> Mat<T, R, C>::operator/(const T& scalar) const {
        return Mat<T, R, C>(*this) /= scalar;
    }

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C>& Mat<T, R, C>::operator/=(const T& scalar) {
        for (auto& element : _m16) {
            element /= scalar;
        }
        return *this;
    }

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C> Mat<T, R, C>::from(const T& scalar) {
        return Mat<T, R, C>(); // TODO
    }

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C> Mat<T, R, C>::from(const Vec<T, R>& vector) {
        return Mat<T, R, C>(); // TODO
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
    template <std::size_t ROW, std::size_t COL, typename>
    constexpr Mat<T, ROW, COL> Mat<T, R, C>::identity() {
        Mat<T, ROW, COL> mat;
        for (unsigned int r = 0; r < ROW; r++) {
            for (unsigned int c = 0; c < COL; c++) {
                mat._m44[r][c] = Math::equal(r, c) ? T(1.0) : T(0.0);
            }
        }
        return mat;
    }

    template <typename T, std::size_t R, std::size_t C>
    constexpr Mat<T, R, C> Mat<T, R, C>::zero() {
        Mat<T, R, C> mat;
        for (unsigned int r = 0; r < R; r++) {
            for (unsigned int c = 0; c < C; c++) {
                mat._m44[r][c] = T(0.0);
            }
        }
        return mat;
    }

    template <typename T, std::size_t R, std::size_t C>
    Vec<T, C> Mat<T, R, C>::diagonal(const Mat<T, R, C>& other) {
        const std::size_t dimension = Math::min(R, C);
        Vec<T, dimension> result;
        for (unsigned int c = 0; c < dimension; c++) {
            result.v[c] = other._m44[c][c];
        }
        return result;
    }

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C> Mat<T, R, C>::transpose(const Mat<T, R, C>& other) {
        Mat<T, R, C> transposed = other;
        for (unsigned int r = 0; r < R; r++) {
            for (unsigned int c = r + 1; c < C; c++) {
                std::swap(transposed._m44[r][c], transposed._m44[c][r]);
            }
        }
        return transposed;
    }

    template <typename T, std::size_t R, std::size_t C>
    T Mat<T, R, C>::minor(const Mat<T, R, C>& other, std::size_t row, std::size_t col) {
        return nullptr; // TODO
    }

    template <typename T, std::size_t R, std::size_t C>
    T Mat<T, R, C>::cofactor(const Mat<T, R, C>& other, std::size_t row, std::size_t col) {
        // Cofactor: (-1)^(row+col) * minor(row,col)
        return Math::pow(real(-1), real(row + col)) * Mat<T, R, C>::minor(other, row, col);
    }

    template <typename T, std::size_t R, std::size_t C>
    T Mat<T, R, C>::determinant(const Mat<T, R, C>& other) {
        // Sum(col=0->n)[A(0,col)*Cofactor(0,col)]
        T result = T(0);
        for (unsigned int c = 0; c < C; c++) {
            result += other._m44[0][c] * Mat<T, R, C>::cofactor(other, 0, c); // fix row, change column <=> fix col, change row
        }
        return result;
    }

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C> Mat<T, R, C>::adjugate(const Mat<T, R, C>& other) {
        Mat<T, R, C> cofactorMatrix;
        for (unsigned int r = 0 ; r < 4 ; r++) {
            for (unsigned int c = 0 ; c < 4 ; c++) {
                cofactorMatrix._m44[r][c] = Mat<T, R, C>::cofactor(other, r, c);
            }
        }
        return Mat<T, R, C>::transpose(cofactorMatrix);
    }

    template <typename T, std::size_t R, std::size_t C>
    template <std::size_t ROW, std::size_t COL, typename>
    Mat<T, ROW, COL> Mat<T, R, C>::inverse(const Mat<T, ROW, COL>& other) {
        T determinant = Mat<T, ROW, COL>::determinant(other);
        if (Math::equal(determinant, T(0))) {
            throw std::logic_error("Determinant is zero! Inverse does not exist.");
        }
        determinant = T(1) / determinant;

        return Mat<T, ROW, COL>::adjugate(other) * determinant;
    }

    template <typename T, std::size_t R, std::size_t C>
    T Mat<T, R, C>::inverseDeterminant(const Mat<T, R, C>& other) {
        // det(A-1) = 1 / det(A)
        T determinant = Mat<T, R, C>::determinant(other);
        if (Math::equal(determinant, T(0))) {
            throw std::logic_error("Determinant is zero! Inverse does not exist.");
        }
        return T(1) / determinant;
    }

    template <typename T, std::size_t R, std::size_t C>
    T Mat<T, R, C>::trace(const Mat<T, R, C>& other) {
        const std::size_t dimension = Math::min(R, C);
        T result;
        for (unsigned int c = 0; c < dimension; c++) {
            result += other._m44[c][c];
        }
        return result;
    }

    template <typename T, std::size_t R, std::size_t C>
    void Mat<T, R, C>::swap(Mat<T, R, C>& first, Mat<T, R, C>& second) {
        using std::swap;

        for (unsigned int r = 0; r < R; r++) {
            for (unsigned int c = 0; c < C; c++) {
                swap(first._m44[r][c], second._m44[r][c]);
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
