#ifndef GLADOS_MAT4_HPP
#define GLADOS_MAT4_HPP

#include <stdexcept>

#include "Angle.hpp"
#include "Math.h"
#include "Quat.h"
#include "UVec3.h"
#include "UVec4.h"
#include "Vec3.h"
#include "Vec4.h"
#include "utils/Utility.h"
#include "platform/OSTypes.h"

namespace GLaDOS {
    /*
     * Example of matrix representation in other Game Engines.
     * - Row major matrix -> DirectX, Unreal Engine
     * - Column major matrix -> OpenGL, Unity, glm
     *
     * GLaDOS's Mat4<T> class represents Row major 4x4 matrix.
     * | a  b  c |   | x |   | ax + by + cz |
     * | d  e  f | * | y | = | dx + ey + fz |
     * | g  h  i |   | z |   | gx + hy + iz |
     *
     * it means that consecutive memory layout places like so
     * real arr[] = { a, b, c,
     *                d, e, f,
     *                g, h, i };
     */
    template <typename T>
    class Mat4 {
      public:
        Mat4();
        ~Mat4() = default;
        Mat4(T m11, T m12, T m13, T m14,
             T m21, T m22, T m23, T m24,
             T m31, T m32, T m33, T m34,
             T m41, T m42, T m43, T m44);
        Mat4(const Vec4& row1, const Vec4& row2, const Vec4& row3, const Vec4& row4);
        Mat4(const Mat4& other);
        Mat4(Mat4<T>&& other) noexcept;
        Mat4<T>& operator=(Mat4<T> other);  // copy and swap idiom

        void makeIdentity();
        bool isIdentity() const;
        Mat4<T>& makeTranspose();
        Mat4<T>& makeInverse();
        T* pointer();

        Mat4<T> operator+(const Mat4<T>& m) const;
        Mat4<T>& operator+=(const Mat4<T>& m);
        Mat4<T> operator-(const Mat4<T>& m) const;
        Mat4<T>& operator-=(const Mat4<T>& m);
        Mat4<T> operator*(const Mat4<T>& m) const;
        Mat4<T>& operator*=(const Mat4<T>& m);
        bool operator==(const Mat4<T>& m) const;
        bool operator!=(const Mat4<T>& m) const;

        // Intentionally don't overload operator * function with vec4
        // because it makes matrix seem like column major matrix (M * v).

        Mat4<T> operator+(const T& scalar) const;
        Mat4<T>& operator+=(const T& scalar);
        Mat4<T> operator-(const T& scalar) const;
        Mat4<T>& operator-=(const T& scalar);
        Mat4<T> operator*(const T& scalar) const;
        Mat4<T>& operator*=(const T& scalar);
        Mat4<T> operator/(const T& scalar) const;
        Mat4<T>& operator/=(const T& scalar);
        Mat4<T>& operator++();
        Mat4<T>& operator--();

        real operator()(unsigned int row, unsigned int col) const;
        real operator[](unsigned int index) const;
        real at(int index) const;
        real at(int row, int col) const;
        std::size_t size() const;
        Vec4 col(unsigned int index) const;
        Vec4 row(unsigned int index) const;

        static constexpr Mat4<T> identity();
        static constexpr Mat4<T> zero();
        static Vec4 diagonal(const Mat4<T>& other);
        static Mat4<T> transpose(const Mat4<T>& other);
        static T minor(const Mat4<T>& other, std::size_t row, std::size_t col);
        static T cofactor(const Mat4<T>& other, std::size_t row, std::size_t col);
        static T determinant(const Mat4<T>& other);
        static Mat4<T> adjugate(const Mat4<T>& other);
        static Mat4<T> inverse(const Mat4<T>& other);
        static T inverseDeterminant(const Mat4<T>& other);
        static Mat4<T> toMat3(const Mat4<T>& other);
        static Mat4<T> abs(const Mat4<T>& other);
        static T trace(const Mat4<T>& other);
        static Mat4<T> elementary1(unsigned int rowIndex, T scalar); // row scalar multiplication
        static Mat4<T> elementary2(unsigned int firstRowIndex, unsigned int secondRowIndex); // row swap
        static Mat4<T> elementary3(unsigned int firstRowIndex, T scalar, unsigned int secondRowIndex); // row scalar multiplication and addition
        static std::enable_if_t<is_real_v<T>, Mat4<T>> perspective(Rad fieldOfView, const T& aspectRatio, const T& znear, const T& zfar);
        static std::enable_if_t<is_real_v<T>, Mat4<T>> orthogonal(const T& left, const T& right, const T& bottom, const T& top, const T& znear, const T& zfar);
        static std::enable_if_t<is_real_v<T>, Mat4<T>> frustum(const T& left, const T& right, const T& bottom, const T& top, const T& znear, const T& zfar);
        static std::enable_if_t<is_real_v<T>, Mat4<T>> lookAt(const Vec3& eye, const Vec3& forward, const UVec3& up);
        static std::enable_if_t<is_real_v<T>, Mat4<T>> translate(const Vec3& trans);
        static std::enable_if_t<is_real_v<T>, Mat4<T>> scale(const Vec3& scale);
        static std::enable_if_t<is_real_v<T>, Mat4<T>> rotate(const Vec3& eulerAngle);
        static std::enable_if_t<is_real_v<T>, Mat4<T>> rotate(const Quat& q);
        static std::enable_if_t<is_real_v<T>, Mat4<T>> normalizeComponents(const Mat4<T>& matrix);
        static std::enable_if_t<is_real_v<T>, Vec3> decomposeTranslation(const Mat4<T>& matrix);
        static std::enable_if_t<is_real_v<T>, Vec3> decomposeRotation(const Mat4<T>& matrix);
        static std::enable_if_t<is_real_v<T>, Vec3> decomposeScale(const Mat4<T>& matrix);
        static std::enable_if_t<is_real_v<T>, Mat4<T>> buildSRT(const Vec3& p, const Quat& q, const Vec3& s);

        union {
            struct {
                T _11, _12, _13, _14;
                T _21, _22, _23, _24;
                T _31, _32, _33, _34;
                T _41, _42, _43, _44;
            };

            T _m44[4][4];
            T _m16[16];
            Vec4 rows[4];
        };

      private:
        static Logger* logger;
        static T determinant3x3(const Mat4<T>& mat3x3);
        static void swap(Mat4& first, Mat4& second);
    };

    template <typename T>
    Logger* Mat4<T>::logger = LoggerRegistry::getInstance().makeAndGetLogger("Mat4");

    template <typename T>
    Mat4<T>::Mat4() {
        for (unsigned c = 0; c < 4; c++) {
            for (unsigned r = 0; r < 4; r++) {
                _m44[c][r] = Math::equal(c, r) ? 1.0 : 0.0;
            }
        }
    }

    template <typename T>
    Mat4<T>::Mat4(T m11, T m12, T m13, T m14,
                  T m21, T m22, T m23, T m24,
                  T m31, T m32, T m33, T m34,
                  T m41, T m42, T m43, T m44) {
        _m44[0][0] = m11;
        _m44[0][1] = m12;
        _m44[0][2] = m13;
        _m44[0][3] = m14;
        _m44[1][0] = m21;
        _m44[1][1] = m22;
        _m44[1][2] = m23;
        _m44[1][3] = m24;
        _m44[2][0] = m31;
        _m44[2][1] = m32;
        _m44[2][2] = m33;
        _m44[2][3] = m34;
        _m44[3][0] = m41;
        _m44[3][1] = m42;
        _m44[3][2] = m43;
        _m44[3][3] = m44;
    }

    template <typename T>
    Mat4<T>::Mat4(const Vec4& row1, const Vec4& row2, const Vec4& row3, const Vec4& row4) {
        rows[0] = row1;
        rows[1] = row2;
        rows[2] = row3;
        rows[3] = row4;
    }

    template <typename T>
    Mat4<T>::Mat4(const Mat4<T>& other) {
        for (unsigned c = 0; c < 4; c++) {
            for (unsigned r = 0; r < 4; r++) {
                _m44[c][r] = other._m44[c][r];
            }
        }
    }

    template <typename T>
    Mat4<T>::Mat4(Mat4<T>&& other) noexcept : Mat4<T>{} {
        Mat4<T>::swap(*this, other);
    }

    template <typename T>
    Mat4<T>& Mat4<T>::operator=(Mat4<T> other) {
        Mat4<T>::swap(*this, other);
        return *this;
    }

    template <typename T>
    void Mat4<T>::makeIdentity() {
        for (unsigned c = 0; c < 4; c++) {
            for (unsigned r = 0; r < 4; r++) {
                _m44[c][r] = Math::equal(c, r) ? 1.0 : 0.0;
            }
        }
    }

    template <typename T>
    bool Mat4<T>::isIdentity() const {
        return *this == Mat4<T>::identity();
    }

    template <typename T>
    Mat4<T>& Mat4<T>::makeTranspose() {
        return *this = Mat4<T>::transpose(*this);
    }

    template <typename T>
    Mat4<T>& Mat4<T>::makeInverse() {
        return *this = Mat4<T>::inverse(*this);
    }

    template <typename T>
    T* Mat4<T>::pointer() {
        return _m16;
    }

    template <typename T>
    Mat4<T> Mat4<T>::operator+(const Mat4<T>& m) const {
        return Mat4(*this) += m;
    }

    template <typename T>
    Mat4<T>& Mat4<T>::operator+=(const Mat4<T>& m) {
        for (unsigned c = 0; c < 4; c++) {
            for (unsigned r = 0; r < 4; r++) {
                _m44[c][r] += m._m44[c][r];
            }
        }

        return *this;
    }

    template <typename T>
    Mat4<T> Mat4<T>::operator-(const Mat4<T>& m) const {
        return Mat4(*this) -= m;
    }

    template <typename T>
    Mat4<T>& Mat4<T>::operator-=(const Mat4<T>& m) {
        for (unsigned c = 0; c < 4; c++) {
            for (unsigned r = 0; r < 4; r++) {
                _m44[c][r] -= m._m44[c][r];
            }
        }

        return *this;
    }

    template <typename T>
    Mat4<T> Mat4<T>::operator*(const Mat4<T>& m) const {
        return Mat4(*this) *= m;
    }

    template <typename T>
    Mat4<T>& Mat4<T>::operator*=(const Mat4<T>& m) {
        Mat4 t{};
        for (unsigned r = 0; r < 4; r++) {
            for (unsigned c = 0; c < 4; c++) {
                t._m44[r][c] = 0.0;
                for (unsigned k = 0; k < 4; k++) {
                    t._m44[r][c] += _m44[r][k] * m._m44[k][c];
                }
            }
        }
        return *this = t;
    }

    template <typename T>
    bool Mat4<T>::operator==(const Mat4<T>& m) const {
        for (unsigned c = 0; c < 4; c++) {
            for (unsigned r = 0; r < 4; r++) {
                if (!Math::equal(_m44[c][r], m._m44[c][r])) {
                    return false;
                }
            }
        }

        return true;
    }

    template <typename T>
    bool Mat4<T>::operator!=(const Mat4<T>& m) const {
        return !(*this == m);  // use equal operator to implement not equal
    }

    template <typename T>
    Mat4<T> Mat4<T>::operator+(const T& scalar) const {
        return Mat4<T>(*this) += scalar;
    }

    template <typename T>
    Mat4<T>& Mat4<T>::operator+=(const T& scalar) {
        for (auto& i : _m16) {
            i += scalar;
        }

        return *this;
    }

    template <typename T>
    Mat4<T> Mat4<T>::operator-(const T& scalar) const {
        return Mat4<T>(*this) -= scalar;
    }

    template <typename T>
    Mat4<T>& Mat4<T>::operator-=(const T& scalar) {
        for (auto& i : _m16) {
            i -= scalar;
        }

        return *this;
    }

    template <typename T>
    Mat4<T> Mat4<T>::operator*(const T& scalar) const {
        return Mat4<T>(*this) *= scalar;
    }

    template <typename T>
    Mat4<T>& Mat4<T>::operator*=(const T& scalar) {
        for (auto& i : _m16) {
            i *= scalar;
        }

        return *this;
    }

    template <typename T>
    Mat4<T> Mat4<T>::operator/(const T& scalar) const {
        return Mat4<T>(*this) /= scalar;
    }

    template <typename T>
    Mat4<T>& Mat4<T>::operator/=(const T& scalar) {
        for (auto& i : _m16) {
            i /= scalar;
        }

        return *this;
    }

    template <typename T>
    Mat4<T>& Mat4<T>::operator++() {
        for (auto& e : _m16) {
            e++;
        }
    }

    template <typename T>
    Mat4<T>& Mat4<T>::operator--() {
        for (auto& e : _m16) {
            e--;
        }
    }

    template <typename T>
    real Mat4<T>::operator()(unsigned int row, unsigned int col) const {
        return _m16[row * 4 + col];
    }

    template <typename T>
    real Mat4<T>::operator[](unsigned int index) const {
        return _m16[index];
    }

    template <typename T>
    real Mat4<T>::at(int index) const {
        if (index >= 0 && index <= 16) {
            return (*this)[index];
        }

        throw std::out_of_range("index is out of range!");
    }

    template <typename T>
    real Mat4<T>::at(int row, int col) const {
        if (row >= 0 && row <= 3 && col >= 0 && col <= 3) {
            return (*this)._m44[row][col];
        }

        throw std::out_of_range("index is out of range!");
    }

    template <typename T>
    std::size_t Mat4<T>::size() const {
        return sizeof(T) * 16;
    }

    template <typename T>
    Vec4 Mat4<T>::col(unsigned int index) const {
        if (index < 4) {
            return Vec4{_m44[0][index], _m44[1][index], _m44[2][index], _m44[3][index]};
        }

        throw std::out_of_range("index is out of range!");
    }

    template <typename T>
    Vec4 Mat4<T>::row(unsigned int index) const {
        if (index < 4) {
            return rows[index];
        }

        throw std::out_of_range("index is out of range!");
    }

    template <typename T>
    constexpr Mat4<T> Mat4<T>::identity() {
        Mat4<T> mat;
        for (unsigned c = 0; c < 4; c++) {
            for (unsigned r = 0; r < 4; r++) {
                mat._m44[c][r] = Math::equal(c, r) ? 1.0 : 0.0;
            }
        }
        return mat;
    }

    template <typename T>
    constexpr Mat4<T> Mat4<T>::zero() {
        Mat4<T> mat;
        for (unsigned c = 0; c < 4; c++) {
            for (unsigned r = 0; r < 4; r++) {
                mat._m44[c][r] = 0.0;
            }
        }
        return mat;
    }

    template <typename T>
    Vec4 Mat4<T>::diagonal(const Mat4<T>& other) {
        return Vec4(other._m44[0][0], other._m44[1][1], other._m44[2][2], other._m44[3][3]);
    }

    template <typename T>
    Mat4<T> Mat4<T>::transpose(const Mat4<T>& other) {
        Mat4<T> mat = other;
        for (unsigned c = 0; c < 4; c++) {
            for (unsigned r = c + 1; r < 4; r++) {
                std::swap(mat._m44[c][r], mat._m44[r][c]);
            }
        }

        return mat;
    }

    template <typename T>
    T Mat4<T>::minor(const Mat4<T>& other, std::size_t row, std::size_t col) {
        Mat4<T> minorMatrix;
        std::size_t minorRow = 0;
        std::size_t minorCol = 0;

        for (std::size_t i = 0; i < 4; i++) {
            if (i != row) {
                for (std::size_t j = 0; j < 4; j++) {
                    if (j != col) {
                        minorMatrix._m44[minorRow][minorCol] = other._m44[i][j];
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

        return determinant3x3(minorMatrix);
    }

    template <typename T>
    T Mat4<T>::cofactor(const Mat4<T>& other, std::size_t row, std::size_t col) {
        // Cofactor Matrix: (-1)^(row+col) * minor(row,col)
        return Math::pow(real(-1), real(row + col)) * Mat4<T>::minor(other, row, col);
    }

    template <typename T>
    T Mat4<T>::determinant(const Mat4<T>& other) {
        // Laplace expansion: Sigma(col=0->n)[A(0,col)*Cofactor(0,col)]
        T result = T(0);
        for (int j = 0; j < 4; j++) {
            result += other._m44[0][j] * Mat4<T>::cofactor(other, 0, j);
        }

        return result;
    }

    template <typename T>
    Mat4<T> Mat4<T>::adjugate(const Mat4<T>& other) {
        Mat4<T> cofactorMatrix;
        for (int i = 0 ; i < 4 ; i++) {
            for (int j = 0 ; j < 4 ; j++) {
                cofactorMatrix._m44[i][j] = Mat4<T>::cofactor(other, i, j);
            }
        }

        return Mat4<T>::transpose(cofactorMatrix);
    }

    template <typename T>
    Mat4<T> Mat4<T>::inverse(const Mat4<T>& other) {
        T determinant = Mat4<T>::determinant(other);
        if (determinant == T(0)) {
            LOG_ERROR(logger, "Matrix determinant is zero! Inverse does not exist.");
            return Mat4<T>::identity();
        }
        determinant = T(1) / determinant;

        return Mat4<T>::adjugate(other) * determinant;
    }

    template <typename T>
    T Mat4<T>::inverseDeterminant(const Mat4<T>& other) {
        // det(A-1) = 1 / det(A)
        T determinant = Mat4<T>::determinant(other);
        if (determinant == T(0)) {
            LOG_ERROR(logger, "Matrix determinant is zero! Inverse does not exist.");
            return T(0);
        }
        return T(1) / determinant;
    }

    template <typename T>
    Mat4<T> Mat4<T>::toMat3(const Mat4<T>& other) {
        auto z = static_cast<T>(0);
        auto o = static_cast<T>(1);

        return Mat4<T>(other._m16[0], other._m16[1], other._m16[2], other._m16[3],
                       other._m16[4], other._m16[5], other._m16[6], other._m16[7],
                       other._m16[8], other._m16[9], other._m16[10], other._m16[11],
                       z, z, z, o);
    }

    template <typename T>
    Mat4<T> Mat4<T>::abs(const Mat4<T>& other) {
        return Mat4<T>(Math::abs(other._m16[0]), Math::abs(other._m16[1]), Math::abs(other._m16[2]), Math::abs(other._m16[3]),
                       Math::abs(other._m16[4]), Math::abs(other._m16[5]), Math::abs(other._m16[6]), Math::abs(other._m16[7]),
                       Math::abs(other._m16[8]), Math::abs(other._m16[9]), Math::abs(other._m16[10]), Math::abs(other._m16[11]),
                       Math::abs(other._m16[12]), Math::abs(other._m16[13]), Math::abs(other._m16[14]), Math::abs(other._m16[15]));
    }

    template <typename T>
    T Mat4<T>::trace(const Mat4<T>& other) {
        return other._m44[0][0] + other._m44[1][1] + other._m44[2][2] + other._m44[3][3];
    }

    template <typename T>
    Mat4<T> Mat4<T>::elementary1(unsigned int rowIndex, T scalar) {
        Mat4<T> elementMatrix;
        if (Math::equal(scalar, T(0))) {
            return elementMatrix;
        }
        for (unsigned int i = 0; i < 4; i++) {
            elementMatrix._m44[rowIndex][i] *= scalar;
        }
        return elementMatrix;
    }

    template <typename T>
    Mat4<T> Mat4<T>::elementary2(unsigned int firstRowIndex, unsigned int secondRowIndex) {
        Mat4<T> elementMatrix;
        for (unsigned int i = 0; i < 4; i++) {
            std::swap(elementMatrix._m44[firstRowIndex][i], elementMatrix._m44[secondRowIndex][i]);
        }
        return elementMatrix;
    }

    template <typename T>
    Mat4<T> Mat4<T>::elementary3(unsigned int firstRowIndex, T scalar, unsigned int secondRowIndex) {
        Mat4<T> elementMatrix;
        if (Math::equal(scalar, T(0))) {
            return elementMatrix;
        }
        for (unsigned int i = 0; i < 4; i++) {
            elementMatrix._m44[secondRowIndex][i] += elementMatrix._m44[firstRowIndex][i] * scalar;
        }
        return elementMatrix;
    }

    template <typename T>
    std::enable_if_t<is_real_v<T>, Mat4<T>> Mat4<T>::perspective(Rad fieldOfView, const T& aspectRatio, const T& znear, const T& zfar) {
        auto tanHalfFovy = Math::tan(static_cast<T>(fieldOfView) / T(2.0));

        /*
            http://metashapes.com/blog/opengl-metal-projection-matrix-problem/
            Metal defines its Normalized Device Coordinate (NDC) system as a 2x2x1 cube with its center at (0, 0, 0.5).
            Post multiplication by 0.5 to have the correct center.
            | 1  0  0    0 |
            | 0  1  0    0 |
            | 0  0  0.5  0 |
            | 0  0  0.5  1 |
        */
        Mat4<real> mat = Mat4<T>::zero();
        mat._m16[0] = T(1.0) / (aspectRatio * tanHalfFovy);
        mat._m16[5] = T(1.0) / tanHalfFovy;
        mat._m16[10] = -(zfar + znear) / (zfar - znear);
        mat._m16[11] = T(-1.0);
        mat._m16[14] = -(T(2.0) * zfar * znear) / (zfar - znear);

        Mat4<T> adjust;
#ifdef PLATFORM_MACOS
        adjust._m16[10] = T(0.5);
        adjust._m16[14] = T(0.5);
#endif

        return mat * adjust;
    }

    template <typename T>
    std::enable_if_t<is_real_v<T>, Mat4<T>> Mat4<T>::orthogonal(const T& left, const T& right, const T& bottom, const T& top, const T& znear, const T& zfar) {
        if (left == right || top == bottom || znear == zfar) {
            LOG_ERROR(logger, "Invalid orthogonal matrix.");
            return Mat4<T>::identity();
        }

        T rl = T(1.0) / (right - left);
        T tb = T(1.0) / (top - bottom);
        T fn = T(1.0) / (zfar - znear);

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
        Mat4<T> mat{};
        mat._m16[0] = T(2.0) * rl;
        mat._m16[5] = T(2.0) * tb;
        mat._m16[10] = T(-2.0) * fn;
        mat._m16[12] = -(right + left) * rl;
        mat._m16[13] = -(top + bottom) * tb;
        mat._m16[14] = -(zfar + znear) * fn;

        Mat4<T> adjust;
#ifdef PLATFORM_MACOS
        adjust._m16[10] = T(0.5);
        adjust._m16[14] = T(0.5);
#endif

        return mat * adjust;
    }

    template <typename T>
    std::enable_if_t<is_real_v<T>, Mat4<T>> Mat4<T>::frustum(const T& left, const T& right, const T& bottom, const T& top, const T& znear, const T& zfar) {
        if (left == right || top == bottom || znear == zfar) {
            LOG_ERROR(logger, "Invalid frustum matrix.");
            return Mat4<T>::identity();
        }

        T rl = T(1.0) / (right - left);
        T tb = T(1.0) / (top - bottom);
        T fn = T(1.0) / (zfar - znear);

        // TODO
        /*  row-major matrix
            | (2*n)/(r-l)       0            0             0        |
            | 0             (2*n)/(t-b)      0             0        |
            | (r+l)/(r-l)   (t+b)/(t-b)   -(f+n)/(f-n)     -1       |
            | 0                 0        -(2*f*n)/(f-n)    0        |
        */
        Mat4<T> mat{};
        mat._m16[0] = T(2) * znear * rl;
        mat._m16[5] = T(2) * znear * tb;

        mat._m16[8] = (right + left) * rl;
        mat._m16[9] = (top + bottom) * tb;
        mat._m16[10] = -(zfar + znear) * fn;
        mat._m16[11] = T(-1);
        mat._m16[14] = -(T(2) * zfar * znear) * fn;
        mat._m16[15] = T(0);

        Mat4<T> adjust;
#ifdef PLATFORM_MACOS
        adjust._m16[10] = T(0.5);
        adjust._m16[14] = T(0.5);
#endif

        return mat * adjust;
    }

    template <typename T>
    std::enable_if_t<is_real_v<T>, Mat4<T>> Mat4<T>::lookAt(const Vec3& eye, const Vec3& forward, const UVec3& up) {
        UVec3 zaxis = Vec3::normalize(eye - forward);
        UVec3 xaxis = Vec3::normalize(Vec3::cross(up, zaxis));
        Vec3 yaxis = Vec3::cross(zaxis, xaxis);

        return Mat4<real>{xaxis->x, yaxis.x, zaxis->x, T(0.0),
                          xaxis->y, yaxis.y, zaxis->y, T(0.0),
                          xaxis->z, yaxis.z, zaxis->z, T(0.0),
                          -Vec3::dot(xaxis, eye), -Vec3::dot(yaxis, eye), -Vec3::dot(zaxis, eye), T(1.0)};
    }

    template <typename T>
    std::enable_if_t<is_real_v<T>, Mat4<T>> Mat4<T>::translate(const Vec3& trans) {
        /*
         |1 0 0 0|
         |0 1 0 0|
         |0 0 1 0|
         |x y z 1|
        */
        Mat4<T> mat;
        mat._m16[12] = trans.x;
        mat._m16[13] = trans.y;
        mat._m16[14] = trans.z;

        return mat;
    }

    template <typename T>
    std::enable_if_t<is_real_v<T>, Mat4<T>> Mat4<T>::scale(const Vec3& scale) {
        /*
         |x 0 0 0|
         |0 y 0 0|
         |0 0 z 0|
         |0 0 0 1|
        */
        Mat4<T> mat;
        mat._m16[0] = scale.x;
        mat._m16[5] = scale.y;
        mat._m16[10] = scale.z;

        return mat;
    }

    template <typename T>
    std::enable_if_t<is_real_v<T>, Mat4<T>> Mat4<T>::rotate(const Vec3& eulerAngle) {
        /*
            Build Euler rotation matrix (possibly, gimbal lock), ZYX order

             Rx = | 1  0    0   0 |
                  | 0 cos -sin  0 |
                  | 0 sin  cos  0 |
                  | 0  0    0   1 |

             Ry = |  cos  0  sin 0 |
                  |   0	 1	 0   0 |
                  | -sin  0  cos 0 |
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
        real cz = Math::cos(Math::toRadians(Deg{eulerAngle.z}).get());
        real sz = Math::sin(Math::toRadians(Deg{eulerAngle.z}).get());

        // Yaw
        real cy = Math::cos(Math::toRadians(Deg{eulerAngle.y}).get());
        real sy = Math::sin(Math::toRadians(Deg{eulerAngle.y}).get());

        // Pitch
        real cx = Math::cos(Math::toRadians(Deg{eulerAngle.x}).get());
        real sx = Math::sin(Math::toRadians(Deg{eulerAngle.x}).get());

        Mat4<T> result;
        result._m44[0][0] = cz * cy;
        result._m44[0][1] = -sz * cx + cz * sy * sx;
        result._m44[0][2] = sz * sx + cz * sy * cx;

        result._m44[1][0] = sz * cy;
        result._m44[1][1] = cz * cx + sz * sy * sx;
        result._m44[1][2] = -cz * sx + sz * sy * cx;

        result._m44[2][0] = -sy;
        result._m44[2][1] = cy * sx;
        result._m44[2][2] = cy * cx;

        return result;
    }

    template <typename T>
    std::enable_if_t<is_real_v<T>, Mat4<T>> Mat4<T>::rotate(const Quat& q) {
        /*
           row-major vector (v)

                      | 1-2y^2-2z^2		2xy-2wz		 2xz+2wy		0 |
                      | 2xy+2wz			1-2x^2-2z^2	 2yz-2wx		0 |
            (x,y,z,0) | 2xz-2wy			2yz+2wx		 1-2x^2-2y^2	0 |
                      | 0				    0			 0			1 |
        */
        Mat4<real> result;

        static real one = real(1.0);
        static real two = real(2.0);

        real xx = q.x * q.x;
        real yy = q.y * q.y;
        real zz = q.z * q.z;
        real xz = q.x * q.z;
        real xy = q.x * q.y;
        real yz = q.y * q.z;
        real wx = q.w * q.x;
        real wy = q.w * q.y;
        real wz = q.w * q.z;

        result._m44[0][0] = one - two * (yy + zz);
        result._m44[0][1] = two * (xy - wz);
        result._m44[0][2] = two * (xz + wy);

        result._m44[1][0] = two * (xy + wz);
        result._m44[1][1] = one - two * (xx + zz);
        result._m44[1][2] = two * (yz - wx);

        result._m44[2][0] = two * (xz - wy);
        result._m44[2][1] = two * (yz + wx);
        result._m44[2][2] = one - two * (xx + yy);

        return result;
    }

    template <typename T>
    std::enable_if_t<is_real_v<T>, Mat4<T>> Mat4<T>::normalizeComponents(const Mat4<T>& matrix) {
        UVec4 first = Vec4::normalize(Vec4{matrix._11, matrix._12, matrix._13, matrix._14});
        UVec4 second = Vec4::normalize(Vec4{matrix._21, matrix._22, matrix._23, matrix._24});
        UVec4 third = Vec4::normalize(Vec4{matrix._31, matrix._32, matrix._33, matrix._34});

        return Mat4<T>{first->x, first->y, first->z, first->w,
                       second->x, second->y, second->z, second->w,
                       third->x, third->y, third->z, third->w,
                       matrix._41, matrix._42, matrix._43, matrix._44};
    }

    template <typename T>
    std::enable_if_t<is_real_v<T>, Vec3> Mat4<T>::decomposeTranslation(const Mat4<T>& matrix) {
        return Vec3{matrix._41, matrix._42, matrix._43};
    }

    template <typename T>
    std::enable_if_t<is_real_v<T>, Vec3> Mat4<T>::decomposeRotation(const Mat4<T>& matrix) {
        Mat4<T> components = Mat4<T>::normalizeComponents(matrix);

        // return to degree of vec3
        return Vec3{Math::toDegrees(Rad{Math::atan2(components._23, components._33)}).get(),
                    Math::toDegrees(Rad{-Math::sin(components._13)}).get(),
                    Math::toDegrees(Rad{Math::atan2(components._12, components._11)}).get()};
    }

    template <typename T>
    std::enable_if_t<is_real_v<T>, Vec3> Mat4<T>::decomposeScale(const Mat4<T>& matrix) {
        real first = Vec3(matrix._11, matrix._12, matrix._13).length();
        real second = Vec3(matrix._21, matrix._22, matrix._23).length();
        real third = Vec3(matrix._31, matrix._32, matrix._33).length();

        return Vec3{first, second, third};
    }

    template <typename T>
    std::enable_if_t<is_real_v<T>, Mat4<T>> Mat4<T>::buildSRT(const Vec3& p, const Quat& q, const Vec3& s) {
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

          Because Mat4<T> class uses row major matrix, SRT is correct multiplication order.
        */
        return Mat4<real>::scale(s) * Mat4<real>::rotate(q) * Mat4<real>::translate(p);
    }

    template <typename T>
    T Mat4<T>::determinant3x3(const Mat4<T>& mat3x3) {
        /*
            |a b c|
            |d e f|
            |g h i|
        */

        T aei = mat3x3._m44[0][0] * mat3x3._m44[1][1] * mat3x3._m44[2][2];
        T afh = mat3x3._m44[0][0] * mat3x3._m44[1][2] * mat3x3._m44[2][1];
        T bfg = mat3x3._m44[0][1] * mat3x3._m44[1][2] * mat3x3._m44[2][0];
        T bdi = mat3x3._m44[0][1] * mat3x3._m44[1][0] * mat3x3._m44[2][2];
        T cdh = mat3x3._m44[0][2] * mat3x3._m44[1][0] * mat3x3._m44[2][1];
        T ceg = mat3x3._m44[0][2] * mat3x3._m44[1][1] * mat3x3._m44[2][0];

        return (aei - afh) + (bfg - bdi) + (cdh - ceg);
    }

    template <typename T>
    void Mat4<T>::swap(Mat4<T>& first, Mat4<T>& second) {
        using std::swap;

        for (int c = 0; c < 4; c++) {
            for (int r = 0; r < 4; r++) {
                swap(first._m44[c][r], second._m44[c][r]);
            }
        }
    }
}  // namespace GLaDOS

#endif  //GLADOS_MAT4_HPP
