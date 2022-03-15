#ifndef GLADOS_MAT_H
#define GLADOS_MAT_H

#include "utils/Stl.h"
#include "utils/Utility.h"
#include "platform/OSTypes.h"
#include "Vec.hpp"
#include "UVec.hpp"
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
        static Mat<T, R, C> elementaryScaling(unsigned int rowIndex, T scalar); // row scalar multiplication
        static Mat<T, R, C> elementaryInterchange(unsigned int firstRowIndex, unsigned int secondRowIndex); // row swap
        static Mat<T, R, C> elementaryReplacement(unsigned int firstRowIndex, unsigned int secondRowIndex, T scalar); // row scalar multiplication and addition

        template<std::size_t ROW = R, std::size_t COL = C, typename = typename std::enable_if_t<ROW == 4 && COL == 4>>
        static std::enable_if_t<is_real_v<T>, Mat<T, ROW, COL>> perspective(Rad fieldOfView, const T& aspectRatio, const T& znear, const T& zfar);
        template<std::size_t ROW = R, std::size_t COL = C, typename = typename std::enable_if_t<ROW == 4 && COL == 4>>
        static std::enable_if_t<is_real_v<T>, Mat<T, ROW, COL>> orthogonal(const T& left, const T& right, const T& bottom, const T& top, const T& znear, const T& zfar);
        template<std::size_t ROW = R, std::size_t COL = C, typename = typename std::enable_if_t<ROW == 4 && COL == 4>>
        static std::enable_if_t<is_real_v<T>, Mat<T, ROW, COL>> frustum(const T& left, const T& right, const T& bottom, const T& top, const T& znear, const T& zfar);
        template<std::size_t ROW = R, std::size_t COL = C, typename = typename std::enable_if_t<ROW == 4 && COL == 4>>
        static std::enable_if_t<is_real_v<T>, Mat<T, ROW, COL>> lookAt(const Vec<T, COL-1>& eye, const Vec<T, COL-1>& forward, const UVec<T, COL-1>& up);
        template<std::size_t ROW = R, std::size_t COL = C, typename = typename std::enable_if_t<ROW == 4 && COL == 4>>
        static std::enable_if_t<is_real_v<T>, Mat<T, ROW, COL>> translate(const Vec<T, COL-1>& trans);
        template<std::size_t ROW = R, std::size_t COL = C, typename = typename std::enable_if_t<ROW == 4 && COL == 4>>
        static std::enable_if_t<is_real_v<T>, Mat<T, ROW, COL>> scale(const Vec<T, COL-1>& scale);
        template<std::size_t ROW = R, std::size_t COL = C, typename = typename std::enable_if_t<ROW == 4 && COL == 4>>
        static std::enable_if_t<is_real_v<T>, Mat<T, ROW, COL>> rotate(const Vec<T, COL-1>& eulerAngle);
        template<std::size_t ROW = R, std::size_t COL = C, typename = typename std::enable_if_t<ROW == 4 && COL == 4>>
        static std::enable_if_t<is_real_v<T>, Mat<T, ROW, COL>> rotate(const Quat& quat);

        template<std::size_t ROW = R, std::size_t COL = C, typename = typename std::enable_if_t<ROW == 4 && COL == 4>>
        static std::enable_if_t<is_real_v<T>, Mat<T, ROW, COL>> normalizeComponents(const Mat<T, ROW, COL>& matrix);
        template<std::size_t ROW = R, std::size_t COL = C, typename = typename std::enable_if_t<ROW == 4 && COL == 4>>
        static std::enable_if_t<is_real_v<T>, Vec<T, COL-1>> decomposeTranslation(const Mat<T, ROW, COL>& matrix);
        template<std::size_t ROW = R, std::size_t COL = C, typename = typename std::enable_if_t<ROW == 4 && COL == 4>>
        static std::enable_if_t<is_real_v<T>, Vec<T, COL-1>> decomposeRotation(const Mat<T, ROW, COL>& matrix);
        template<std::size_t ROW = R, std::size_t COL = C, typename = typename std::enable_if_t<ROW == 4 && COL == 4>>
        static std::enable_if_t<is_real_v<T>, Vec<T, COL-1>> decomposeScale(const Mat<T, ROW, COL>& matrix);
        template<std::size_t ROW = R, std::size_t COL = C, typename = typename std::enable_if_t<ROW == 4 && COL == 4>>
        static std::enable_if_t<is_real_v<T>, Mat<T, ROW, COL>> buildSRT(const Vec<T, COL-1>& translation, const Quat& rotation, const Vec<T, COL-1>& scale);

        union {
            T _m44[R][C];
            T _m16[R*C];
            Vec<T, C> rows[R];
        };

      private:
        template<std::size_t ROW, std::size_t COL>
        struct Determinant {
            static T determinant(const Mat<T, ROW, COL>& other);
        };

        template<>
        struct Determinant<2, 2> {
            static T determinant(const Mat<T, 2, 2>& other) {
                // ad - bc
                return other._m44[0][0] * other._m44[1][1] - other._m44[0][1] * other._m44[1][0];
            }
        };

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
        return Determinant<ROW, COL>::determinant(other);
    }

    template <typename T, std::size_t R, std::size_t C>
    template <std::size_t ROW, std::size_t COL>
    T Mat<T, R, C>::Determinant<ROW, COL>::determinant(const Mat<T, ROW, COL>& other) {
        // Sum(col=0->n)[A(0,col)*Cofactor(0,col)]
        T result = T(0);
        for (unsigned int c = 0; c < COL; c++) {
            result += other._m44[0][c] * Mat<T, ROW, COL>::cofactor(other, 0, c);
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
            throw std::logic_error("Matrix is singular.");
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
            throw std::logic_error("Determinant is zero.");
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
    Mat<T, R, C> Mat<T, R, C>::elementaryScaling(unsigned int rowIndex, T scalar) {
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
    Mat<T, R, C> Mat<T, R, C>::elementaryInterchange(unsigned int firstRowIndex, unsigned int secondRowIndex) {
        Mat<T, R, C> elementMatrix;
        for (unsigned int c = 0; c < C; c++) {
            std::swap(elementMatrix._m44[firstRowIndex][c], elementMatrix._m44[secondRowIndex][c]);
        }
        return elementMatrix;
    }

    template <typename T, std::size_t R, std::size_t C>
    Mat<T, R, C> Mat<T, R, C>::elementaryReplacement(unsigned int firstRowIndex, unsigned int secondRowIndex, T scalar) {
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
    template <std::size_t ROW, std::size_t COL, typename>
    std::enable_if_t<is_real_v<T>, Mat<T, ROW, COL>> Mat<T, R, C>::perspective(Rad fieldOfView, const T& aspectRatio, const T& znear, const T& zfar) {
        /*
            http://metashapes.com/blog/opengl-metal-projection-matrix-problem/
            Metal defines its Normalized Device Coordinate (NDC) system as a 2x2x1 cube with its center at (0, 0, 0.5).
            Post multiplication by 0.5 to have the correct center.
            | 1  0  0    0 |
            | 0  1  0    0 |
            | 0  0  0.5  0 |
            | 0  0  0.5  1 |
        */
        Mat<T, ROW, COL> mat = Mat<T, ROW, COL>::zero();
        auto tanHalfFovy = Math::tan(static_cast<T>(fieldOfView) / T(2.0));

        mat._m16[0] = T(1.0) / (aspectRatio * tanHalfFovy);
        mat._m16[5] = T(1.0) / tanHalfFovy;
        mat._m16[10] = -(zfar + znear) / (zfar - znear);
        mat._m16[11] = T(-1.0);
        mat._m16[14] = -(T(2.0) * zfar * znear) / (zfar - znear);

#ifdef PLATFORM_MACOS
        Mat<T, ROW, COL> adjust;
        adjust._m16[10] = T(0.5);
        adjust._m16[14] = T(0.5);
        return mat * adjust;
#else
        return mat;
#endif
    }

    template <typename T, std::size_t R, std::size_t C>
    template <std::size_t ROW, std::size_t COL, typename>
    std::enable_if_t<is_real_v<T>, Mat<T, ROW, COL>> Mat<T, R, C>::orthogonal(const T& left, const T& right, const T& bottom, const T& top, const T& znear, const T& zfar) {
        /*  row-major matrix
            | 2/(r-l)       0            0             0            |
            | 0             2/(t-b)      0             0            |
            | 0             0            -2/(f-n)      -(f+n)/(f-n) |
            | -(r+l)/(r-l)  -(t+b)/t-b)  -(f+n)/(f-n)  1            |

            http://metashapes.com/blog/opengl-metal-projection-matrix-problem/
            Metal defines its Normalized Device Coordinate (NDC) system as a 2x2x1 cube with its center at (0, 0, 0.5).
            Post multiplication by 0.5 to have the correct center.
            | 1  0  0    0 |
            | 0  1  0    0 |
            | 0  0  0.5  0 |
            | 0  0  0.5  1 |
        */
        if (left == right || top == bottom || znear == zfar) {
            throw std::logic_error("Invalid orthogonal matrix.");
        }

        T rl = T(1.0) / (right - left);
        T tb = T(1.0) / (top - bottom);
        T fn = T(1.0) / (zfar - znear);

        Mat<T, ROW, COL> mat;
        mat._m16[0] = T(2.0) * rl;
        mat._m16[5] = T(2.0) * tb;
        mat._m16[10] = T(-2.0) * fn;
        mat._m16[12] = -(right + left) * rl;
        mat._m16[13] = -(top + bottom) * tb;
        mat._m16[14] = -(zfar + znear) * fn;

#ifdef PLATFORM_MACOS
        Mat<T, ROW, COL> adjust;
        adjust._m16[10] = T(0.5);
        adjust._m16[14] = T(0.5);
        return mat * adjust;
#else
        return mat;
#endif
    }

    template <typename T, std::size_t R, std::size_t C>
    template <std::size_t ROW, std::size_t COL, typename>
    std::enable_if_t<is_real_v<T>, Mat<T, ROW, COL>> Mat<T, R, C>::frustum(const T& left, const T& right, const T& bottom, const T& top, const T& znear, const T& zfar) {
        /*
            row-major matrix
            | (2*n)/(r-l)       0            0             0        |
            | 0             (2*n)/(t-b)      0             0        |
            | (r+l)/(r-l)   (t+b)/(t-b)   -(f+n)/(f-n)     -1       |
            | 0                 0        -(2*f*n)/(f-n)    0        |

            http://metashapes.com/blog/opengl-metal-projection-matrix-problem/
            Metal defines its Normalized Device Coordinate (NDC) system as a 2x2x1 cube with its center at (0, 0, 0.5).
            Post multiplication by 0.5 to have the correct center.
            | 1  0  0    0 |
            | 0  1  0    0 |
            | 0  0  0.5  0 |
            | 0  0  0.5  1 |
        */
        if (left == right || top == bottom || znear == zfar) {
            throw std::logic_error("Invalid frustum matrix.");
        }

        T rl = T(1.0) / (right - left);
        T tb = T(1.0) / (top - bottom);
        T fn = T(1.0) / (zfar - znear);

        Mat<T, ROW, COL> mat;
        mat._m16[0] = T(2) * znear * rl;
        mat._m16[5] = T(2) * znear * tb;

        mat._m16[8] = (right + left) * rl;
        mat._m16[9] = (top + bottom) * tb;
        mat._m16[10] = -(zfar + znear) * fn;
        mat._m16[11] = T(-1);
        mat._m16[14] = -(T(2) * zfar * znear) * fn;
        mat._m16[15] = T(0);

#ifdef PLATFORM_MACOS
        Mat<T, ROW, COL> adjust;
        adjust._m16[10] = T(0.5);
        adjust._m16[14] = T(0.5);
        return mat * adjust;
#else
        return mat;
#endif
    }

    template <typename T, std::size_t R, std::size_t C>
    template <std::size_t ROW, std::size_t COL, typename>
    std::enable_if_t<is_real_v<T>, Mat<T, ROW, COL>> Mat<T, R, C>::lookAt(const Vec<T, COL-1>& eye, const Vec<T, COL-1>& forward, const UVec<T, COL-1>& up) {
        UVec<T, COL-1> zaxis = Vec<T, COL-1>::normalize(eye - forward);
        UVec<T, COL-1> xaxis = Vec<T, COL-1>::normalize(Vec<T, COL-1>::cross(up, zaxis));
        Vec<T, COL-1> yaxis = Vec<T, COL-1>::cross(zaxis, xaxis);

        return Mat<T, ROW, COL>{xaxis->x, yaxis.x, zaxis->x, T(0.0),
                                xaxis->y, yaxis.y, zaxis->y, T(0.0),
                                xaxis->z, yaxis.z, zaxis->z, T(0.0),
                                -Vec<T, COL-1>::dot(xaxis, eye), -Vec<T, COL-1>::dot(yaxis, eye), -Vec<T, COL-1>::dot(zaxis, eye), T(1.0)};
    }

    template <typename T, std::size_t R, std::size_t C>
    template <std::size_t ROW, std::size_t COL, typename>
    std::enable_if_t<is_real_v<T>, Mat<T, ROW, COL>> Mat<T, R, C>::translate(const Vec<T, COL-1>& trans) {
        /*
            |1 0 0 0|
            |0 1 0 0|
            |0 0 1 0|
            |x y z 1|
        */
        Mat<T, ROW, COL> mat;
        mat._m16[12] = trans.x;
        mat._m16[13] = trans.y;
        mat._m16[14] = trans.z;
        return mat;
    }

    template <typename T, std::size_t R, std::size_t C>
    template <std::size_t ROW, std::size_t COL, typename>
    std::enable_if_t<is_real_v<T>, Mat<T, ROW, COL>> Mat<T, R, C>::scale(const Vec<T, COL-1>& scale) {
        /*
            |x 0 0 0|
            |0 y 0 0|
            |0 0 z 0|
            |0 0 0 1|
        */
        Mat<T, ROW, COL> mat;
        mat._m16[0] = scale.x;
        mat._m16[5] = scale.y;
        mat._m16[10] = scale.z;
        return mat;
    }

    template <typename T, std::size_t R, std::size_t C>
    template <std::size_t ROW, std::size_t COL, typename>
    std::enable_if_t<is_real_v<T>, Mat<T, ROW, COL>> Mat<T, R, C>::rotate(const Vec<T, COL-1>& eulerAngle) {
        /*
            Build Euler rotation matrix (possibly, gimbal lock), ZYX order

            Rx = | 1  0    0   0 |
                 | 0 cos -sin  0 |
                 | 0 sin  cos  0 |
                 | 0  0    0   1 |

            Ry = |  cos  0  sin 0  |
                 |   0	 1	 0   0 |
                 | -sin  0  cos 0  |
                 |   0	 0	 0   1 |

            Rz = | cos -sin  0  0 |
                 | sin  cos  0  0 |
                 |  0    0   1  0 |
                 |  0    0   0  1 |

            R(zyx) = Rz * Ry * Rx

                    Rz             Ry             Rx
            |Cz -Sz  0  0| | Cy  0 Sy   0| |1  0   0  0|   | CzCy   -SzCx+CzSySx  SzSx+CzSyCx  0|
            |Sz  Cz  0  0| |  0  1  0   0| |0 Cx -Sx  0|   | SzCy    CzCx+SzSySx -CzSx+SzSyCx  0|
            | 0   0  1  0|*|-Sy  0 Cy   0|*|0 Sx  Cx  0| = | -Sy        CySx          CyCx     0|
            | 0   0  0  1| |  0  0  0   1| |0  0   0  1|   |  0          0             0       1|
        */

        // Roll
        T cz = Math::cos(Math::toRadians(Deg{eulerAngle.z}).get());
        T sz = Math::sin(Math::toRadians(Deg{eulerAngle.z}).get());

        // Yaw
        T cy = Math::cos(Math::toRadians(Deg{eulerAngle.y}).get());
        T sy = Math::sin(Math::toRadians(Deg{eulerAngle.y}).get());

        // Pitch
        T cx = Math::cos(Math::toRadians(Deg{eulerAngle.x}).get());
        T sx = Math::sin(Math::toRadians(Deg{eulerAngle.x}).get());

        Mat<T, ROW, COL> mat;
        mat._m44[0][0] = cz * cy;
        mat._m44[0][1] = -sz * cx + cz * sy * sx;
        mat._m44[0][2] = sz * sx + cz * sy * cx;

        mat._m44[1][0] = sz * cy;
        mat._m44[1][1] = cz * cx + sz * sy * sx;
        mat._m44[1][2] = -cz * sx + sz * sy * cx;

        mat._m44[2][0] = -sy;
        mat._m44[2][1] = cy * sx;
        mat._m44[2][2] = cy * cx;

        return mat;
    }

    template <typename T, std::size_t R, std::size_t C>
    template <std::size_t ROW, std::size_t COL, typename>
    std::enable_if_t<is_real_v<T>, Mat<T, ROW, COL>> Mat<T, R, C>::rotate(const Quat& quat) {
        /*
            row-major vector (v)

                      | 1-2y^2-2z^2		2xy-2wz		 2xz+2wy		0 |
                      | 2xy+2wz			1-2x^2-2z^2	 2yz-2wx		0 |
            (x,y,z,0) | 2xz-2wy			2yz+2wx		 1-2x^2-2y^2	0 |
                      | 0				    0			 0		    1 |
        */
        Mat<T, ROW, COL> mat;

        static T zero = T(0.0);
        static T one = T(1.0);
        static T two = T(2.0);

        T xx = quat.x * quat.x;
        T yy = quat.y * quat.y;
        T zz = quat.z * quat.z;
        T xz = quat.x * quat.z;
        T xy = quat.x * quat.y;
        T yz = quat.y * quat.z;
        T wx = quat.w * quat.x;
        T wy = quat.w * quat.y;
        T wz = quat.w * quat.z;

        mat._m44[0][0] = one - two * (yy + zz);
        mat._m44[0][1] = two * (xy - wz);
        mat._m44[0][2] = two * (xz + wy);

        mat._m44[1][0] = two * (xy + wz);
        mat._m44[1][1] = one - two * (xx + zz);
        mat._m44[1][2] = two * (yz - wx);

        mat._m44[2][0] = two * (xz - wy);
        mat._m44[2][1] = two * (yz + wx);
        mat._m44[2][2] = one - two * (xx + yy);

        return mat;
    }

    template <typename T, std::size_t R, std::size_t C>
    template <std::size_t ROW, std::size_t COL, typename>
    std::enable_if_t<is_real_v<T>, Mat<T, ROW, COL>> Mat<T, R, C>::normalizeComponents(const Mat<T, ROW, COL>& matrix) {
        UVec<T, COL> first = Vec<T, COL>::normalize(Vec<T, COL>{matrix._m44[0][0], matrix._m44[0][1], matrix._m44[0][2], matrix._m44[0][3]});
        UVec<T, COL> second = Vec<T, COL>::normalize(Vec<T, COL>{matrix._m44[1][0], matrix._m44[1][1], matrix._m44[1][2], matrix._m44[1][3]});
        UVec<T, COL> third = Vec<T, COL>::normalize(Vec<T, COL>{matrix._m44[2][0], matrix._m44[2][1], matrix._m44[2][2], matrix._m44[2][3]});

        return Mat<T, ROW, COL>{first->x, first->y, first->z, first->w,
                                second->x, second->y, second->z, second->w,
                                third->x, third->y, third->z, third->w,
                                matrix._m44[3][0], matrix._m44[3][1], matrix._m44[3][2], matrix._m44[3][3]};
    }

    template <typename T, std::size_t R, std::size_t C>
    template <std::size_t ROW, std::size_t COL, typename>
    std::enable_if_t<is_real_v<T>, Vec<T, COL-1>> Mat<T, R, C>::decomposeTranslation(const Mat<T, ROW, COL>& matrix) {
        return Vec<T, COL-1>{matrix._m44[3][0], matrix._m44[3][1], matrix._m44[3][2]};
    }

    template <typename T, std::size_t R, std::size_t C>
    template <std::size_t ROW, std::size_t COL, typename>
    std::enable_if_t<is_real_v<T>, Vec<T, COL-1>> Mat<T, R, C>::decomposeRotation(const Mat<T, ROW, COL>& matrix) {
        Mat<T, ROW, COL> components = Mat<T, ROW, COL>::normalizeComponents(matrix);

        // return to degree of vec
        return Vec<T, COL-1>{Math::toDegrees(Rad{Math::atan2(components._m44[1][2], components._m44[2][2])}).get(),
                             Math::toDegrees(Rad{-Math::sin(components._m44[0][2])}).get(),
                             Math::toDegrees(Rad{Math::atan2(components._m44[0][1], components._m44[0][0])}).get()};
    }

    template <typename T, std::size_t R, std::size_t C>
    template <std::size_t ROW, std::size_t COL, typename>
    std::enable_if_t<is_real_v<T>, Vec<T, COL-1>> Mat<T, R, C>::decomposeScale(const Mat<T, ROW, COL>& matrix) {
        T first = Vec<T, COL-1>(matrix._m44[0][0], matrix._m44[0][1], matrix._m44[0][2]).length();
        T second = Vec<T, COL-1>(matrix._m44[1][0], matrix._m44[1][1], matrix._m44[1][2]).length();
        T third = Vec<T, COL-1>(matrix._m44[2][0], matrix._m44[2][1], matrix._m44[2][2]).length();

        return Vec<T, COL-1>{first, second, third};
    }

    template <typename T, std::size_t R, std::size_t C>
    template <std::size_t ROW, std::size_t COL, typename>
    std::enable_if_t<is_real_v<T>, Mat<T, ROW, COL>> Mat<T, R, C>::buildSRT(const Vec<T, COL-1>& translation, const Quat& rotation, const Vec<T, COL-1>& scale) {
        /*
            if using column vectors, column major matrix
                        <-- multiply order
            M := T * R * S * | v1 |
                             | v2 |
                             | v3 |
                             |  0 |

            if using row vectors, row major matrix
            M := |v1, v2, v3, 0| * S * R * T
                 multiply order -->

            Because Mat<T, R, C> class uses row major matrix, SRT order is order of multiplication.
        */
        return Mat<T, ROW, COL>::scale(scale) * Mat<T, ROW, COL>::rotate(rotation) * Mat<T, ROW, COL>::translate(translation);
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
