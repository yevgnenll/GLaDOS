#ifndef GAMEENGINE_MAT4_HPP
#define GAMEENGINE_MAT4_HPP

#include <stdexcept>

#include "Angle.hpp"
#include "Math.h"
#include "UVec3.h"
#include "UVec4.h"
#include "Vec3.h"
#include "Vec4.h"

namespace GameEngine {
  template <typename T>
  class Mat4 {
  public:
    Mat4();
    Mat4(T m11, T m12, T m13, T m14,
         T m21, T m22, T m23, T m24,
         T m31, T m32, T m33, T m34,
         T m41, T m42, T m43, T m44);
    Mat4(const Mat4& other);
    Mat4<T>& operator=(Mat4<T> other);

    void makeIdentity();
    bool isIdentity() const;
    Mat4<T>& makeTranspose();
    Mat4<T>& makeInverse();
    T* pointer() const;

    Mat4<T> operator+(const Mat4<T>& m) const;
    Mat4<T>& operator+=(const Mat4<T>& m);
    Mat4<T> operator-(const Mat4<T>& m) const;
    Mat4<T>& operator-=(const Mat4<T>& m);
    Mat4<T> operator*(const Mat4<T>& m) const;
    Mat4<T>& operator*=(const Mat4<T>& m);
    bool operator==(const Mat4<T>& m) const;
    bool operator!=(const Mat4<T>& m) const;

    Vec4 operator*(const Vec4& v);
    Mat4<T>& operator*=(const Vec4& v);

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

    static constexpr Mat4<T> identity();
    static Mat4<T> inverse(const Mat4<T>& other);
    static Mat4<T> toMat3(const Mat4<T>& other);
    static Mat4<T> abs(const Mat4<T>& other);
    static Mat4<real> perspective(Rad fieldOfView, real aspectRatio, real znear, real zfar);
    static Mat4<real> orthogonal(real left, real right, real bottom, real top, real znear, real zfar);
    static Mat4<real> lookAt(const Vec3& eye, const UVec3& forward, const UVec3& up);
    static Mat4<real> translate(const Vec3& trans);
    static Mat4<real> scale(const Vec3& scale);
    static Mat4<real> rotate(Rad angle, const UVec3& axis);
    static Mat4<real> normalizeComponents(const Mat4<real>& matrix);
    static Vec3 decomposeTranslation(const Mat4<real>& matrix);
    static Vec3 decomposeRotation(const Mat4<real>& matrix);
    static Vec3 decomposeScale(const Mat4<real>& matrix);
    static Mat4<real> buildTRS(const Vec3& p, const Vec3& q, const Vec3& s);

    union {
      struct {
        T _11, _12, _13, _14;
        T _21, _22, _23, _24;
        T _31, _32, _33, _34;
        T _41, _42, _43, _44;
      };

      T _m44[4][4];
      T _m16[16];
    };
    static const Mat4<T> one;
    static const Mat4<T> zero;

  private:
    static void swap(Mat4& first, Mat4& second);
  };

  template <typename T>
  const Mat4<T> Mat4<T>::one(1, 1, 1, 1,
                             1, 1, 1, 1,
                             1, 1, 1, 1,
                             1, 1, 1, 1);
  template <typename T>
  const Mat4<T> Mat4<T>::zero(0, 0, 0, 0,
                              0, 0, 0, 0,
                              0, 0, 0, 0,
                              0, 0, 0, 0);

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
  Mat4<T>::Mat4(const Mat4<T>& other) {
    for (unsigned c = 0; c < 4; c++) {
      for (unsigned r = 0; r < 4; r++) {
        _m44[c][r] = other._m44[c][r];
      }
    }
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
    for (unsigned c = 0; c < 4; c++) {
      for (unsigned r = c + 1; r < 4; r++) {
        std::swap(_m44[c][r], _m44[r][c]);
      }
    }
    return *this;
  }

  template <typename T>
  Mat4<T>& Mat4<T>::makeInverse() {
    return *this = Mat4<T>::inverse(*this);
  }

  template <typename T>
  T* Mat4<T>::pointer() const {
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
  Vec4 Mat4<T>::operator*(const Vec4& v) {
    //TODO
    return Vec4();
  }

  template <typename T>
  Mat4<T>& Mat4<T>::operator*=(const Vec4& v) {
    //TODO
    return *this;
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
      return (*this).m44[row][col];
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
  Mat4<T> Mat4<T>::inverse(const Mat4<T>& other) {
    // 		   | a  b |
    //   A = | c  d |

    //	       	  1  | d  -b |
    //   A^-1 = ad-bc| -c  a |
    real determinant =
        other._m16[0] * other._m16[5] * other._m16[10] * other._m16[15] + other._m16[0] * other._m16[6] * other._m16[11] * other._m16[13] + other._m16[0] * other._m16[7] * other._m16[9] * other._m16[14] +
        other._m16[1] * other._m16[4] * other._m16[11] * other._m16[14] + other._m16[1] * other._m16[6] * other._m16[8] * other._m16[15] + other._m16[1] * other._m16[7] * other._m16[10] * other._m16[12] +
        other._m16[2] * other._m16[4] * other._m16[9] * other._m16[15] + other._m16[2] * other._m16[5] * other._m16[11] * other._m16[12] + other._m16[2] * other._m16[7] * other._m16[8] * other._m16[13] +
        other._m16[3] * other._m16[4] * other._m16[10] * other._m16[13] + other._m16[3] * other._m16[5] * other._m16[8] * other._m16[14] + other._m16[3] * other._m16[6] * other._m16[9] * other._m16[12] -
        other._m16[0] * other._m16[5] * other._m16[11] * other._m16[14] - other._m16[0] * other._m16[6] * other._m16[9] * other._m16[15] - other._m16[0] * other._m16[7] * other._m16[10] * other._m16[13] -
        other._m16[1] * other._m16[4] * other._m16[10] * other._m16[15] - other._m16[1] * other._m16[6] * other._m16[11] * other._m16[12] - other._m16[1] * other._m16[7] * other._m16[8] * other._m16[14] -
        other._m16[2] * other._m16[4] * other._m16[11] * other._m16[13] - other._m16[2] * other._m16[5] * other._m16[8] * other._m16[15] - other._m16[2] * other._m16[7] * other._m16[9] * other._m16[12] -
        other._m16[3] * other._m16[4] * other._m16[9] * other._m16[14] - other._m16[3] * other._m16[5] * other._m16[10] * other._m16[12] - other._m16[3] * other._m16[6] * other._m16[8] * other._m16[13];

    if (determinant == 0) {
      throw std::logic_error("determinant is zero! Inverse does not exist.");
    }

    Mat4<T> mat = Mat4<T>(other._m16[5] * other._m16[10] * other._m16[15] + other._m16[6] * other._m16[11] * other._m16[13] + other._m16[7] * other._m16[9] * other._m16[14] -
                              other._m16[5] * other._m16[11] * other._m16[14] - other._m16[6] * other._m16[9] * other._m16[15] - other._m16[7] * other._m16[10] * other._m16[13],
                          other._m16[1] * other._m16[11] * other._m16[14] + other._m16[2] * other._m16[9] * other._m16[15] + other._m16[3] * other._m16[10] * other._m16[13] -
                              other._m16[1] * other._m16[10] * other._m16[15] - other._m16[2] * other._m16[11] * other._m16[13] - other._m16[3] * other._m16[9] * other._m16[14],
                          other._m16[1] * other._m16[6] * other._m16[15] + other._m16[2] * other._m16[7] * other._m16[13] + other._m16[3] * other._m16[5] * other._m16[14] -
                              other._m16[1] * other._m16[7] * other._m16[14] - other._m16[2] * other._m16[5] * other._m16[15] - other._m16[3] * other._m16[6] * other._m16[13],
                          other._m16[1] * other._m16[7] * other._m16[10] + other._m16[2] * other._m16[5] * other._m16[11] + other._m16[3] * other._m16[6] * other._m16[9] -
                              other._m16[1] * other._m16[6] * other._m16[11] - other._m16[2] * other._m16[7] * other._m16[9] - other._m16[3] * other._m16[5] * other._m16[10],
                          other._m16[4] * other._m16[11] * other._m16[14] + other._m16[6] * other._m16[8] * other._m16[15] + other._m16[7] * other._m16[10] * other._m16[12] -
                              other._m16[4] * other._m16[10] * other._m16[15] - other._m16[6] * other._m16[11] * other._m16[12] - other._m16[7] * other._m16[8] * other._m16[14],
                          other._m16[0] * other._m16[10] * other._m16[15] + other._m16[2] * other._m16[11] * other._m16[12] + other._m16[3] * other._m16[8] * other._m16[14] -
                              other._m16[0] * other._m16[11] * other._m16[14] - other._m16[2] * other._m16[8] * other._m16[15] - other._m16[3] * other._m16[10] * other._m16[12],
                          other._m16[0] * other._m16[7] * other._m16[14] + other._m16[2] * other._m16[4] * other._m16[15] + other._m16[3] * other._m16[6] * other._m16[12] -
                              other._m16[0] * other._m16[6] * other._m16[15] - other._m16[2] * other._m16[7] * other._m16[12] - other._m16[3] * other._m16[4] * other._m16[14],
                          other._m16[0] * other._m16[6] * other._m16[11] + other._m16[2] * other._m16[7] * other._m16[8] + other._m16[3] * other._m16[4] * other._m16[10] -
                              other._m16[0] * other._m16[7] * other._m16[10] - other._m16[2] * other._m16[4] * other._m16[11] - other._m16[3] * other._m16[6] * other._m16[8],
                          other._m16[4] * other._m16[9] * other._m16[15] + other._m16[5] * other._m16[11] * other._m16[12] + other._m16[7] * other._m16[8] * other._m16[13] -
                              other._m16[4] * other._m16[11] * other._m16[13] - other._m16[5] * other._m16[8] * other._m16[15] - other._m16[7] * other._m16[9] * other._m16[12],
                          other._m16[0] * other._m16[11] * other._m16[13] + other._m16[1] * other._m16[8] * other._m16[15] + other._m16[3] * other._m16[9] * other._m16[12] -
                              other._m16[0] * other._m16[9] * other._m16[15] - other._m16[1] * other._m16[11] * other._m16[12] - other._m16[3] * other._m16[8] * other._m16[13],
                          other._m16[0] * other._m16[5] * other._m16[15] + other._m16[1] * other._m16[7] * other._m16[12] + other._m16[3] * other._m16[4] * other._m16[13] -
                              other._m16[0] * other._m16[7] * other._m16[13] - other._m16[1] * other._m16[4] * other._m16[15] - other._m16[3] * other._m16[5] * other._m16[12],
                          other._m16[0] * other._m16[7] * other._m16[9] + other._m16[1] * other._m16[4] * other._m16[11] + other._m16[3] * other._m16[5] * other._m16[8] -
                              other._m16[0] * other._m16[5] * other._m16[11] - other._m16[1] * other._m16[7] * other._m16[8] - other._m16[3] * other._m16[4] * other._m16[9],
                          other._m16[4] * other._m16[10] * other._m16[13] + other._m16[5] * other._m16[8] * other._m16[14] + other._m16[6] * other._m16[9] * other._m16[12] -
                              other._m16[4] * other._m16[9] * other._m16[14] - other._m16[5] * other._m16[10] * other._m16[12] - other._m16[6] * other._m16[8] * other._m16[13],
                          other._m16[0] * other._m16[9] * other._m16[14] + other._m16[1] * other._m16[10] * other._m16[12] + other._m16[2] * other._m16[8] * other._m16[13] -
                              other._m16[0] * other._m16[10] * other._m16[13] - other._m16[1] * other._m16[8] * other._m16[14] - other._m16[2] * other._m16[9] * other._m16[12],
                          other._m16[0] * other._m16[6] * other._m16[13] + other._m16[1] * other._m16[4] * other._m16[14] + other._m16[2] * other._m16[5] * other._m16[12] -
                              other._m16[0] * other._m16[5] * other._m16[14] - other._m16[1] * other._m16[6] * other._m16[12] - other._m16[2] * other._m16[4] * other._m16[13],
                          other._m16[0] * other._m16[5] * other._m16[10] + other._m16[1] * other._m16[6] * other._m16[8] + other._m16[2] * other._m16[4] * other._m16[9] -
                              other._m16[0] * other._m16[6] * other._m16[9] - other._m16[1] * other._m16[4] * other._m16[10] - other._m16[2] * other._m16[5] * other._m16[8]);
    return mat * (1 / determinant);
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

  template <>
  Mat4<real> Mat4<real>::perspective(Rad fieldOfView, real aspectRatio, real znear, real zfar) {
    real tanHalfFovy = Math::tan(static_cast<real>(fieldOfView) / 2.f);

    Mat4<real> mat = Mat4<real>::zero;
    mat._m16[0] = 1.0f / (aspectRatio * tanHalfFovy);
    mat._m16[5] = 1.0f / tanHalfFovy;
    mat._m16[10] = -(zfar + znear) / (zfar - znear);
    mat._m16[11] = -1.0f;
    mat._m16[14] = -(2.0f * zfar * znear) / (zfar - znear);

    return mat;
  }

  template <>
  Mat4<real> Mat4<real>::orthogonal(real left, real right, real bottom, real top, real znear, real zfar) {
    real rl = 1.0f / (right - left);
    real tb = 1.0f / (top - bottom);
    real fn = 1.0f / (zfar - znear);

    Mat4<real> mat{};
    mat._m16[0] = 2.f * rl;
    mat._m16[5] = 2.f * tb;
    mat._m16[10] = -2.f * fn;
    mat._m16[12] = -(right + left) * rl;
    mat._m16[13] = -(top + bottom) * tb;
    mat._m16[14] = -(zfar + znear) * fn;

    return mat;
  }

  template <>
  Mat4<real> Mat4<real>::lookAt(const Vec3& eye, const UVec3& forward, const UVec3& up) {
    UVec3 zaxis = Vec3::normalize((eye - forward));
    UVec3 xaxis = Vec3::normalize(Vec3::cross(up, zaxis));
    Vec3 yaxis = Vec3::cross(zaxis, xaxis);

    return Mat4<real>(xaxis->x, yaxis.x, zaxis->x, 0.F,
                      xaxis->y, yaxis.y, zaxis->y, 0.F,
                      xaxis->z, yaxis.z, zaxis->z, 0.F,
                      -Vec3::dot(xaxis, eye), -Vec3::dot(yaxis, eye), -Vec3::dot(zaxis, eye), 1.F);
  }

  template <>
  Mat4<real> Mat4<real>::translate(const Vec3& trans) {
    /*
     |1 0 0 0|
     |0 1 0 0|
     |0 0 1 0|
     |x y z 1|
    */
    Mat4<real> mat;
    mat._m16[12] = trans.x;
    mat._m16[13] = trans.y;
    mat._m16[14] = trans.z;

    return mat;
  }

  template <>
  Mat4<real> Mat4<real>::scale(const Vec3& scale) {
    /*
     |x 0 0 0|
     |0 y 0 0|
     |0 0 z 0|
     |0 0 0 1|
    */
    Mat4<real> mat;
    mat._m16[0] = scale.x;
    mat._m16[5] = scale.y;
    mat._m16[10] = scale.z;

    return mat;
  }

  template <>
  Mat4<real> Mat4<real>::rotate(Rad angle, const UVec3& axis) {
    /*
     x = | 1  0    0   0 |
         | 0 cos -sin  0 |
         | 0 sin  cos  0 |
         | 0  0    0   1 |

     y = |  cos  0  sin  0 |
         |   0	 1	 0   0 |
         | -sin  0  cos	 0 |
         |   0	 0	 0   1 |

     z = | cos -sin  0  0 |
         | sin  cos  0  0 |
         |  0    0   1  0 |
         |  0    0   0  1 |
    */
    real c = Math::cos(static_cast<real>(angle));
    real s = Math::sin(static_cast<real>(angle));
    real t = 1.F - c;

    real tx = t * axis->x;
    real ty = t * axis->y;
    real tz = t * axis->z;

    real sx = s * axis->x;
    real sy = s * axis->y;
    real sz = s * axis->z;

    return Mat4<real>(tx * axis->x + c, tx * axis->y + sz, tx * axis->z - sy, 0.F,
                      ty * axis->x - sz, ty * axis->y + c, ty * axis->z + sx, 0.F,
                      tz * axis->x + sy, tz * axis->y - sx, tz * axis->z + c, 0.F,
                      0.F, 0.F, 0.F, 1.F);
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

  template <>
  Mat4<real> Mat4<real>::normalizeComponents(const Mat4<real>& matrix) {
    UVec4 first = Vec4::normalize(Vec4{matrix._11, matrix._12, matrix._13, matrix._14});
    UVec4 second = Vec4::normalize(Vec4{matrix._21, matrix._22, matrix._23, matrix._24});
    UVec4 third = Vec4::normalize(Vec4{matrix._31, matrix._32, matrix._33, matrix._34});

    return Mat4<real>(first->x, first->y, first->z, first->w,
                      second->x, second->y, second->z, second->w,
                      third->x, third->y, third->z, third->w,
                      matrix._41, matrix._42, matrix._43, matrix._44);
  }

  template <>
  Vec3 Mat4<real>::decomposeTranslation(const Mat4<real>& matrix) {
    return Vec3{matrix._41, matrix._42, matrix._43};
  }

  template <>
  Vec3 Mat4<real>::decomposeRotation(const Mat4<real>& matrix) {
    Mat4<real> components = Mat4<real>::normalizeComponents(matrix);

    // In radian unit
    return Vec3{Math::atan2(components._23, components._33), -Math::sin(components._13), Math::atan2(components._12, components._11)};
  }

  template <>
  Vec3 Mat4<real>::decomposeScale(const Mat4<real>& matrix) {
    real first = Vec3(matrix._11, matrix._12, matrix._13).length();
    real second = Vec3(matrix._21, matrix._22, matrix._23).length();
    real third = Vec3(matrix._31, matrix._32, matrix._33).length();

    return Vec3{first, second, third};
  }

  template <>
  Mat4<real> Mat4<real>::buildTRS(const Vec3& p, const Vec3& q, const Vec3& s) {
    /*
      if using column vectors
      M := T * R * S
      else if using row vectors
      M := S * R * T
    */
    // TODO
    //    return Mat4<real>::scale(s) * Quaternion::ToRotationMatrix(q) * Mat4<real>::translate(p);
    return Mat4<real>{};
  }

  using Mat4r = Mat4<real>;
  using Mat4i = Mat4<int>;
  using Mat4l = Mat4<long>;
}  // namespace GameEngine

#endif  //GAMEENGINE_MAT4_HPP
