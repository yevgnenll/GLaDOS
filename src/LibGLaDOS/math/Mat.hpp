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
        template<std::size_t ROW = R, std::size_t COL = C, typename = typename std::enable_if_t<ROW == COL>>
        Mat<T, R, C>& makeInverse();
        T* pointer();

        Mat<T, R, C> operator+(const Mat<T, R, C>& other) const;
        Mat<T, R, C>& operator+=(const Mat<T, R, C>& other);
        Mat<T, R, C> operator-(const Mat<T, R, C>& other) const;
        Mat<T, R, C>& operator-=(const Mat<T, R, C>& other);
        template<std::size_t R2, std::size_t C2, typename = typename std::enable_if_t<C == R2>>
        Mat<T, R, C2> operator*(const Mat<T, R2, C2>& other) const;
        bool operator==(const Mat<T, R, C>& other) const;
        bool operator!=(const Mat<T, R, C>& other) const;

        // Intentionally don't overload operator * function with vec4
        // because it makes matrix seem like column major matrix (M * v).

        Mat<T, R, C> operator*(const T& scalar) const;
        Mat<T, R, C>& operator*=(const T& scalar);
        Mat<T, R, C> operator/(const T& scalar) const;
        Mat<T, R, C>& operator/=(const T& scalar);

        T operator()(unsigned int row, unsigned int col) const;
        Vec<T, C> operator[](unsigned int index) const;
        T at(int row, int col) const;
        T at(int index) const;
        Vec<T, R> col(unsigned int index) const;
        Vec<T, C> row(unsigned int index) const;

        static Mat<T, R, C> from(const T& scalar);
        static Mat<T, R, C> from(const Vec<T, C>& vector);
        static Mat<T, R, C> from(const T (&scalars)[R * C]);
        static constexpr std::size_t dimension();
        static constexpr std::size_t size();
        template<std::size_t ROW = R, std::size_t COL = C, typename = typename std::enable_if_t<ROW == COL>>
        static constexpr Mat<T, ROW, COL> identity();
        static constexpr Mat<T, R, C> zero();
        static Vec<T, C> diagonal(const Mat<T, R, C>& other);
        static Mat<T, C, R> transpose(const Mat<T, R, C>& other);
        template<std::size_t ROW = R, std::size_t COL = C, typename = typename std::enable_if_t<ROW == COL>>
        static T minor(const Mat<T, ROW, COL>& other, std::size_t row, std::size_t col);
        template<std::size_t ROW = R, std::size_t COL = C, typename = typename std::enable_if_t<ROW == COL>>
        static T cofactor(const Mat<T, ROW, COL>& other, std::size_t row, std::size_t col);
        template<std::size_t ROW = R, std::size_t COL = C, typename = typename std::enable_if_t<ROW == COL>>
        static T determinant(const Mat<T, ROW, COL>& other);
        template<std::size_t ROW = R, std::size_t COL = C, typename = typename std::enable_if_t<ROW == COL>>
        static Mat<T, ROW, COL> adjugate(const Mat<T, ROW, COL>& other);
        template<std::size_t ROW = R, std::size_t COL = C, typename = typename std::enable_if_t<ROW == COL>>
        static Mat<T, ROW, COL> inverse(const Mat<T, ROW, COL>& other);
        template<std::size_t ROW = R, std::size_t COL = C, typename = typename std::enable_if_t<ROW == COL>>
        static T inverseDeterminant(const Mat<T, ROW, COL>& other);
        template<std::size_t ROW, std::size_t COL, typename = typename std::enable_if_t<ROW == COL && R == C>>
        static Mat<T, ROW, COL> toSquareMat(const Mat<T, R, C>& other);
        static T trace(const Mat<T, R, C>& other);
        static Mat<T, R, C> elementary1(unsigned int rowIndex, T scalar); // row scalar multiplication
        static Mat<T, R, C> elementary2(unsigned int firstRowIndex, unsigned int secondRowIndex); // row swap
        static Mat<T, R, C> elementary3(unsigned int firstRowIndex, unsigned int secondRowIndex, T scalar); // row scalar multiplication and addition

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
    template <std::size_t ROW, std::size_t COL, typename>
    Mat<T, R, C>& Mat<T, R, C>::makeInverse() {
        return *this = Mat<T, R, C>::inverse(*this);
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
        Mat<T, R, C2> result;
        for (unsigned int r = 0; r < R; r++) {
            for (unsigned int c = 0; c < C2; c++) {
                result._m44[r][c] = T(0);
                for (unsigned int k = 0; k < C; k++) {
                    result._m44[r][c] += _m44[r][k] * other._m44[k][c];
                }
            }
        }
        return result;
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
    T Mat<T, R, C>::operator()(unsigned int row, unsigned int col) const {
        return _m44[row][col];
    }

    template <typename T, std::size_t R, std::size_t C>
    Vec<T, C> Mat<T, R, C>::operator[](unsigned int index) const {
        return rows[index];
    }

    template <typename T, std::size_t R, std::size_t C>
    T Mat<T, R, C>::at(int row, int col) const {
        if (row >= 0 && row <= R && col >= 0 && col <= C) {
            return _m44[row][col];
        }

        throw std::out_of_range("index out of range!");
    }

    template <typename T, std::size_t R, std::size_t C>
    T Mat<T, R, C>::at(int index) const {
        if (index >= 0 && index <= (R * C)) {
            return _m16[index];
        }

        throw std::out_of_range("index out of range!");
    }

    template <typename T, std::size_t R, std::size_t C>
    Vec<T, R> Mat<T, R, C>::col(unsigned int index) const {
        if (index < C) {
            Vec<T, R> vec;
            for (unsigned int r = 0; r < R; r++) {
                vec.v[r] = _m44[r][index];
            }
            return vec;
        }

        throw std::out_of_range("index out of range!");
    }

    template <typename T, std::size_t R, std::size_t C>
    Vec<T, C> Mat<T, R, C>::row(unsigned int index) const {
        if (index < R) {
            return rows[index];
        }

        throw std::out_of_range("index out of range!");
    }

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C> Mat<T, R, C>::from(const T& scalar) {
        Mat<T, R, C> result;
        for (unsigned int r = 0; r < R; r++) {
            for (unsigned int c = 0; c < C; c++) {
                result._m44[r][c] = scalar;
            }
        }
        return result;
    }

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C> Mat<T, R, C>::from(const Vec<T, C>& vector) {
        Mat<T, R, C> result;
        for (unsigned int r = 0; r < R; r++) {
            result.rows[r] = vector;
        }
        return result;
    }

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C> Mat<T, R, C>::from(const T (&scalars)[R * C]) {
        Mat<T, R, C> result;
        for (unsigned int r = 0; r < R; r++) {
            for (unsigned int c = 0; c < C; c++) {
                result._m44[r][c] = scalars[r * C + c];
            }
        }
        return result;
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
    Mat<T, C, R> Mat<T, R, C>::transpose(const Mat<T, R, C>& other) {
        T temp[R * C] = {0, };
        for (unsigned int n = 0; n < R * C; n++) {
            int i = n / R;
            int j = n % R;
            temp[n] = other._m16[C * j + i];
        }
        return Mat<T, C, R>::from(temp);
    }

    template <typename T, std::size_t R, std::size_t C>
    template <std::size_t ROW, std::size_t COL, typename>
    T Mat<T, R, C>::minor(const Mat<T, ROW, COL>& other, std::size_t row, std::size_t col) {
        Mat<T, R-1, C-1> minorMatrix;
        std::size_t minorRow = 0;
        std::size_t minorCol = 0;

        for (unsigned int r = 0; r < ROW; r++) {
            if (r != row) {
                for (unsigned int c = 0; c < COL; c++) {
                    if (c != col) {
                        minorMatrix._m44[minorRow][minorCol] = other._m44[r][c];
                        minorCol++;
                    }
                }
                minorCol = 0;
                minorRow++;
            }
            if (minorRow > 2) {
                minorRow = 0;
            }
        }

        return Mat<T, R-1, C-1>::determinant(minorMatrix);
    }

    template <typename T, std::size_t R, std::size_t C>
    template <std::size_t ROW, std::size_t COL, typename>
    T Mat<T, R, C>::cofactor(const Mat<T, ROW, COL>& other, std::size_t row, std::size_t col) {
        // Cofactor: (-1)^(row+col) * minor(row,col)
        return Math::pow(real(-1), real(row + col)) * Mat<T, ROW, COL>::minor(other, row, col);
    }

    template <typename T, std::size_t R, std::size_t C>
    template <std::size_t ROW, std::size_t COL, typename>
    T Mat<T, R, C>::determinant(const Mat<T, ROW, COL>& other) {
        // Sum(col=0->n)[A(0,col)*Cofactor(0,col)]
        T result = T(0);
        for (unsigned int c = 0; c < COL; c++) {
            result += other._m44[0][c] * Mat<T, ROW, COL>::cofactor(other, 0, c); // fix row, change column <=> fix col, change row
        }
        return result;
    }

    template <typename T, std::size_t R, std::size_t C>
    template <std::size_t ROW, std::size_t COL, typename>
    Mat<T, ROW, COL> Mat<T, R, C>::adjugate(const Mat<T, ROW, COL>& other) {
        Mat<T, ROW, COL> cofactorMatrix;
        for (unsigned int r = 0 ; r < R; r++) {
            for (unsigned int c = 0 ; c < C; c++) {
                cofactorMatrix._m44[r][c] = Mat<T, ROW, COL>::cofactor(other, r, c);
            }
        }
        return Mat<T, ROW, COL>::transpose(cofactorMatrix);
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
    template <std::size_t ROW, std::size_t COL, typename>
    T Mat<T, R, C>::inverseDeterminant(const Mat<T, ROW, COL>& other) {
        // det(A-1) = 1 / det(A)
        T determinant = Mat<T, ROW, COL>::determinant(other);
        if (Math::equal(determinant, T(0))) {
            throw std::logic_error("Determinant is zero! Inverse does not exist.");
        }
        return T(1) / determinant;
    }

    template <typename T, std::size_t R, std::size_t C>
    template <std::size_t ROW, std::size_t COL, typename>
    Mat<T, ROW, COL> Mat<T, R, C>::toSquareMat(const Mat<T, R, C>& other) {
        Mat<T, ROW, COL> result;
        const std::size_t minRow = Math::min(R, ROW);
        const std::size_t minCol = Math::min(C, COL);

        for (unsigned int r = 0; r < minRow; r++) {
            for (unsigned int c = 0; c < minCol; c++) {
                result._m44[r][c] = other._m44[r][c];
            }
        }
        return result;
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
    Mat<T, R, C> Mat<T, R, C>::elementary1(unsigned int rowIndex, T scalar) {
        Mat<T, R, C> elementMatrix;
        if (Math::equal(scalar, T(0))) {
            return elementMatrix;
        }
        for (unsigned int c = 0; c < C; c++) {
            elementMatrix._m44[rowIndex][c] *= scalar;
        }
        return elementMatrix;
    }

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C> Mat<T, R, C>::elementary2(unsigned int firstRowIndex, unsigned int secondRowIndex) {
        Mat<T, R, C> elementMatrix;
        for (unsigned int c = 0; c < C; c++) {
            std::swap(elementMatrix._m44[firstRowIndex][c], elementMatrix._m44[secondRowIndex][c]);
        }
        return elementMatrix;
    }

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C> Mat<T, R, C>::elementary3(unsigned int firstRowIndex, unsigned int secondRowIndex, T scalar) {
        Mat<T, R, C> elementMatrix;
        if (Math::equal(scalar, T(0))) {
            return elementMatrix;
        }
        for (unsigned int c = 0; c < C; c++) {
            elementMatrix._m44[secondRowIndex][c] += elementMatrix._m44[firstRowIndex][c] * scalar;
        }
        return elementMatrix;
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
