#ifndef GAMEENGINE_QUAT_H
#define GAMEENGINE_QUAT_H

#include "utils/Enumeration.h"

namespace GameEngine {
  class Vec3;
  class Quat {
  public:
    Quat() = default;
    Quat(float _w, float _x, float _y, float _z);
    Quat(float _w, const Vec3& _v);

    Quat(const Quat& other) = default;
    Quat& operator=(Quat other);

    Quat operator+(const Quat& other) const;
    Quat& operator+=(const Quat& other);

    Quat operator-(const Quat& other) const;
    Quat& operator-=(const Quat& other);

    Quat operator*(const Quat& other) const;
    Quat& operator*=(const Quat& other);

    Quat operator*(const Vec3& other) const;
    Quat& operator*=(const Vec3& other);
    //    Vec3 operator*(const Vec3& other) const;

    Quat operator+(const float scalar) const;
    Quat& operator+=(const float scalar);

    Quat operator-(const float scalar) const;
    Quat& operator-=(const float scalar);

    Quat operator*(const float scalar) const;
    Quat& operator*=(const float scalar);

    Quat operator/(const float scalar) const;
    Quat& operator/=(const float scalar);

    bool operator==(const Quat& other) const;
    bool operator!=(const Quat& other) const;

    real& operator[](unsigned int i);
    const real& operator[](unsigned int i) const;

    Quat& operator*=(const real& s);
    Quat& operator/=(const real& s);

    // vector(imaginary) part, real part
    real w{1.F}, x{0.F}, y{0.F}, z{0.F};

    static const Quat zero, identity;

  private:
    static void swap(Quat& first, Quat& second);
  };
}  // namespace GameEngine

#endif  //GAMEENGINE_QUAT_H
