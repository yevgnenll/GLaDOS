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

        Quat operator-() const;
        Quat operator-(const Quat& other) const;
        Quat& operator-=(const Quat& other);

        Quat operator*(const Quat& other) const;
        Quat& operator*=(const Quat& other);

        // operator/(const Quat&) is not defined. because it is same as Quat * Quat-1 (inverse)

        Vec3 operator*(const Vec3& other) const; // rotated Vec3 with quaternion multiplication
        Vec4 operator*(const Vec4& other) const; // rotated Vec4 with quaternion multiplication

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
        real squaredLength() const;

        static real dot(const Quat& a, const Quat& b);
        static Quat normalize(const Quat& q);
        static Quat inverse(const Quat& q);
        static Quat conjugate(const Quat& q);
        static Vec3 toEuler(const Quat& q); // returns Vec3 of degrees (ZYX order)
        static Quat fromEuler(const Vec3& euler); // degree euler vector (ZYX order)
        static Quat angleAxis(Deg angle, const UVec3& axis);

        static Quat fromRotation(const Mat4<real>& m);
        static Quat fromToRotation(const Vec3& from, const Vec3& to);
        static Quat lookRotation(const Vec3& direction, const Vec3& up = Vec3::up);
        static Deg angleBetween(const Quat& q, const Quat& p); // returns the angle in degrees between two Quat q and p.

        static Quat lerp(const Quat& a, const Quat& b, real t);
        static Quat nlerp(const Quat& a, const Quat& b, real t);
        static Quat slerp(const Quat& a, const Quat& b, real t);

        // real part, vector(imaginary) part
        union {
            struct {
                real w;
                real x;
                real y;
                real z;
            };
            real v[4];
        };
        static const Quat zero, identity;

      private:
        static void swap(Quat& first, Quat& second);
    };
}  // namespace GLaDOS

#endif  //GLADOS_QUAT_H
