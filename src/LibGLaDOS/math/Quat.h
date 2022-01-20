#ifndef GLADOS_QUAT_H
#define GLADOS_QUAT_H

#include "Angle.hpp"
#include "utils/Enumeration.h"
#include "Vec3.h"

namespace GLaDOS {
    class UVec3;
    class Vec4;
    template <typename T>
    class Mat4;
    class Quat {
      public:
        Quat();
        ~Quat() = default;
        Quat(real _w, real _x, real _y, real _z);
        Quat(real scalar, const Vec3& vector);
        Quat(Quat&& other) noexcept;
        Quat(const Quat& other) = default;
        Quat& operator=(Quat other);  // copy and swap idiom

        Quat operator+(const Quat& other) const;
        Quat& operator+=(const Quat& other);

        Quat operator-(const Quat& other) const;
        Quat& operator-=(const Quat& other);

        Quat operator*(const Quat& other) const;
        Quat& operator*=(const Quat& other);

        Vec3 operator*(const Vec3& other) const;
        Vec4 operator*(const Vec4& other) const;

        Quat operator*(const real& scalar) const;
        Quat& operator*=(const real& scalar);

        Quat operator/(const real& scalar) const;
        Quat& operator/=(const real& scalar);

        bool operator==(const Quat& other) const;
        bool operator!=(const Quat& other) const;

        real& operator[](unsigned int i);
        const real& operator[](unsigned int i) const;

        Quat& makeConjugate();
        Quat& makeNormalize();
        Quat& makeInverse();
        real length() const;
        Vec3 conjugate(const Vec3& v) const;

        static Vec3 cross(const Vec3& v, const Quat& q);
        static Vec3 cross(const Quat& q, const Vec3& v);
        static real dot(const Quat& a, const Quat& b);
        static Quat normalize(const Quat& q);
        static Quat inverse(const Quat& q);
        static Quat conjugate(const Quat& q);
        static Vec3 toEuler(const Quat& q);
        static Quat fromEuler(const Vec3& euler);
        static Quat angleAxis(Rad angle, const UVec3& axis);
        static Quat fromToRotation(const Vec3& from, const Vec3& to);
        static Mat4<real> toRotationMat(const Quat& q);
        static Quat fromRotationMat(const Mat4<real>& m);
        // Quaternion nlerp is not defined because it's same as lerp.
        static Quat lerp(const Quat& a, const Quat& b, real t);
        static Quat slerp(const Quat& a, const Quat& b, real t);

        // real part, vector(imaginary) part
        union {
            struct {
                real w;
                real x;
                real y;
                real z;
            };
            struct {
                real scalar;
                Vec3 vector;
            };
            real v[4];
        };
        static const Quat zero, identity;

      private:
        static void swap(Quat& first, Quat& second);
    };
}  // namespace GLaDOS

#endif  //GLADOS_QUAT_H
