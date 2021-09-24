#include "Quat.h"

#include "Mat4.hpp"
#include "Math.h"
#include "Vec3.h"

namespace GLaDOS {
    Quat::Quat(real _w, real _x, real _y, real _z) : w{_w}, x{_x}, y{_y}, z{_z} {}

    Quat::Quat(real _w, const Vec3& _v) : w{_w}, x{_v.x}, y{_v.y}, z{_v.z} {}

    Quat::Quat(Quat&& other) noexcept : Quat{} {
        Quat::swap(*this, other);
    }

    Quat& Quat::operator=(Quat other) {
        // copy and swap idiom (effective c++ section 11)
        Quat::swap(*this, other);
        return *this;
    }

    Quat Quat::operator+(const Quat& other) const {
        // using op= (more effective c++ section 22)
        return Quat{*this} += other;
    }

    Quat& Quat::operator+=(const Quat& other) {
        w += other.w;
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Quat Quat::operator-(const Quat& other) const {
        return Quat{*this} -= other;
    }

    Quat& Quat::operator-=(const Quat& other) {
        w -= other.w;
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Quat Quat::operator*(const Quat& other) const {
        return Quat{*this} *= other;
    }

    Quat& Quat::operator*=(const Quat& other) {
        Quat tmp;

        tmp.w = (other.w * w) - (other.x * x) - (other.y * y) - (other.z * z);
        tmp.x = (other.w * x) + (other.x * w) + (other.z * y) - (other.y * z);
        tmp.y = (other.w * y) + (other.y * w) + (other.x * z) - (other.z * x);
        tmp.z = (other.w * z) + (other.z * w) + (other.y * x) - (other.x * y);

        return *this = tmp;
    }

    Vec3 Quat::operator*(const Vec3& other) const {
        // https://answers.unity.com/questions/372371/multiply-quaternion-by-vector3-how-is-done.html
        static real one = real(1.0);
        static real two = real(2.0);
        real num = x * two;
        real num2 = y * two;
        real num3 = z * two;
        real num4 = x * num;
        real num5 = y * num2;
        real num6 = z * num3;
        real num7 = x * num2;
        real num8 = x * num3;
        real num9 = y * num3;
        real num10 = w * num;
        real num11 = w * num2;
        real num12 = w * num3;

        Vec3 result;
        result.x = (one - (num5 + num6)) * other.x + (num7 - num12) * other.y + (num8 + num11) * other.z;
        result.y = (num7 + num12) * other.x + (one - (num4 + num6)) * other.y + (num9 - num10) * other.z;
        result.z = (num8 - num11) * other.x + (num9 + num10) * other.y + (one - (num4 + num5)) * other.z;

        return result;
    }

    Vec4 Quat::operator*(const Vec4& other) const {
        return Vec4{*this * Vec3{other}, other.w};
    }

    Quat Quat::operator*(const real& scalar) const {
        return Quat{*this} *= scalar;
    }

    Quat& Quat::operator*=(const real& scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }

    Quat Quat::operator/(const real& scalar) const {
        return Quat{*this} /= scalar;
    }

    Quat& Quat::operator/=(const real& scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;
        return *this;
    }

    bool Quat::operator==(const Quat& other) const {
        return (x == other.x && y == other.y && z == other.z && w == other.w);
    }

    bool Quat::operator!=(const Quat& other) const {
        return !(*this == other);
    }

    real& Quat::operator[](unsigned int i) {
        switch (i) {
            case 0:
                return w;
            case 1:
                return x;
            case 2:
                return y;
            case 3:
                return z;
            default:
                return w;
        }
    }

    const real& Quat::operator[](unsigned int i) const {
        switch (i) {
            case 0:
                return w;
            case 1:
                return x;
            case 2:
                return y;
            case 3:
                return z;
            default:
                return w;
        }
    }

    Quat& Quat::makeConjugate() {
        x = -x;
        y = -y;
        z = -z;
        return *this;
    }

    Quat& Quat::makeNormalize() {
        return *this = Quat::normalize(*this);
    }

    Quat& Quat::makeInverse() {
        return makeConjugate() /= Quat::dot(*this, *this);
    }

    real Quat::length() const {
        return Math::sqrt(x * x + y * y + z * z + w * w);
    }

    Vec3 Quat::conjugate(const Vec3& v) const {
        Quat q2{0.F, v.x, v.y, v.z};
        Quat q = *this;
        Quat qinv = q;
        q.makeConjugate();

        q = q * q2 * qinv;
        return Vec3{q.x, q.y, q.z};
    }

    Vec3 Quat::cross(const Vec3& v, const Quat& q) {
        return Quat::inverse(q) * v;
    }

    Vec3 Quat::cross(const Quat& q, const Vec3& v) {
        return q * v;
    }

    real Quat::dot(const Quat& a, const Quat& b) {
        return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
    }

    Quat Quat::normalize(const Quat& q) {
        real n = q.length();
        if (n <= real(0.0)) {
            return Quat::identity;
        }

        real inv = real(1.0) / n;
        return Quat{q.w * inv, q.x * inv, q.y * inv, q.z * inv};
    }

    Quat Quat::inverse(const Quat& q) {
        return Quat{q}.makeInverse();
    }

    Quat Quat::conjugate(const Quat& q) {
        return Quat{q.w, -q.x, -q.y, -q.z};
    }

    Vec3 Quat::toEuler(const Quat& q) {
        return Vec3{Math::pitch(q).get(), Math::yaw(q).get(), Math::roll(q).get()};
    }

    Quat Quat::fromEuler(const Vec3& euler) {
        /*
       Suppose euler is in radian unit.

       Qx = [ cos(a/2), (sin(a/2), 0, 0)]
       Qy = [ cos(b/2), (0, sin(b/2), 0)]
       Qz = [ cos(c/2), (0, 0, sin(c/2))]

       Qx * Qy * Qz
    */
        Quat qx(Math::cos(euler.x * real(0.5)), Math::sin(euler.x * real(0.5)), real(0.0), real(0.0));
        Quat qy(Math::cos(euler.y * real(0.5)), real(0.0), Math::sin(euler.y * real(0.5)), real(0.0));
        Quat qz(Math::cos(euler.z * real(0.5)), real(0.0), real(0.0), Math::sin(euler.z * real(0.5)));

        return qx * qy * qz;
    }

    Quat Quat::angleAxis(Rad angle, const UVec3& axis) {
        auto halfAngle = angle * real(0.5);
        auto halfSin = Math::sin(halfAngle.get());

        return Quat{
            Math::cos(halfAngle.get()),
            halfSin * axis->x,
            halfSin * axis->y,
            halfSin * axis->z};
    }

    Quat Quat::fromToRot(const Vec3& from, const Vec3& to) {
        UVec3 v0 = Vec3::normalize(from);
        UVec3 v1 = Vec3::normalize(to);

        auto d = Vec3::dot(v0, v1);
        if (d >= real(1.0)) {
            return identity;
        }

        if (d <= real(-1.0)) {
            // exactly opposite
            Vec3 v2 = Vec3::cross(Vec3::right, v0);
            if (v2.length() == 0) {
                v2 = Vec3::cross(Vec3::up, v0);
            }
            return Quat{0.0, v2.x, v2.y, v2.z}.makeNormalize();
        }

        real s = Math::sqrt((1 + d) * 2);
        real inv = 1 / s;
        Vec3 v3 = Vec3::cross(v0, v1) * inv;
        return Quat{s * real(0.5), v3.x, v3.y, v3.z}.makeNormalize();
    }

    Mat4<real> Quat::toRotMat(const Quat& q) {
        /*
       1-2y^2-2z^2		2xy-2wz		 2xz+2wy		0
       2xy+2wz			1-2x^2-2z^2	 2yz-2wx		0
       2xz-2wy			2yz+2wx		 1-2x^2-2y^2	0
       0				0			 0				1
    */
        Mat4<real> result;

        real xx = q.x * q.x;
        real yy = q.y * q.y;
        real zz = q.z * q.z;
        real xz = q.x * q.z;
        real xy = q.x * q.y;
        real yz = q.y * q.z;
        real wx = q.w * q.x;
        real wy = q.w * q.y;
        real wz = q.w * q.z;

        result._m44[0][0] = real(1.0) - real(2.0) * (yy + zz);
        result._m44[0][1] = real(2.0) * (xy - wz);
        result._m44[0][2] = real(2.0) * (xz + wy);
        result._m44[0][3] = real(0.0);

        result._m44[1][0] = real(2.0) * (xy + wz);
        result._m44[1][1] = real(1.0) - real(2.0) * (xx + zz);
        result._m44[1][2] = real(2.0) * (yz - wx);
        result._m44[1][3] = real(0.0);

        result._m44[2][0] = real(2.0) * (xz - wy);
        result._m44[2][1] = real(2.0) * (yz + wx);
        result._m44[2][2] = real(1.0) - real(2.0) * (xx + yy);
        result._m44[2][3] = real(0.0);

        result._m44[3][0] = real(0.0);
        result._m44[3][1] = real(0.0);
        result._m44[3][2] = real(0.0);
        result._m44[3][3] = real(1.0);

        return result;
    }

    Quat Quat::fromRotMat(const Mat4<real>& m) {
        real w = Math::sqrt(m._m44[0][0] + m._m44[1][1] + m._m44[2][2] + 1.0F) * real(0.5);
        real x = (m._m44[2][1] - m._m44[1][2]) / (4 * w);
        real y = (m._m44[0][2] - m._m44[2][0]) / (4 * w);
        real z = (m._m44[1][0] - m._m44[0][1]) / (4 * w);

        return Quat{w, x, y, z};
    }

    Quat Quat::lerp(const Quat& a, const Quat& b, real t) {
        auto s = 1.F - t;
        return ((a * s) + (b * t));
    }

    Quat Quat::slerp(const Quat& a, const Quat& b, real t) {
        Quat c{b};
        real angle = dot(a, b);

        // If cosTheta < 0, the interpolation will take the long way around the sphere.
        // To fix this, one quat must be negated.
        if (angle < 0.F) {
            c = b * -1.F;
            angle *= -1.F;
        }

        if (angle <= (1 - real(0.05))) {
            real theta = Math::acos(angle);
            return (a * Math::sin((1.F - t) * theta) + c * Math::sin(t * theta)) / Math::sin(theta);
        }

        // Perform a linear interpolation when cosTheta is close to 1 to avoid side effect of sin(angle)
        // becoming a zero denominator
        return lerp(a, b, t);
    }

    void Quat::swap(Quat& first, Quat& second) {
        using std::swap;

        swap(first.x, second.x);
        swap(first.y, second.y);
        swap(first.z, second.z);
        swap(first.w, second.w);
    }

    const Quat Quat::zero(0.0, 0.0, 0.0, 0.0);
    const Quat Quat::identity(1.0, 0.0, 0.0, 0.0);
}  // namespace GLaDOS
