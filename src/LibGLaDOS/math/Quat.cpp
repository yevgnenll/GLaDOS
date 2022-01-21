#include "Quat.h"

#include "Mat4.hpp"
#include "Math.h"
#include "Vec3.h"

namespace GLaDOS {
    Quat::Quat() : w{1.0}, x{0.0}, y{0.0}, z{0.0} {}

    Quat::Quat(real _w, real _x, real _y, real _z) : w{_w}, x{_x}, y{_y}, z{_z} {}

    Quat::Quat(real scalar, const Vec3& vector) : w{scalar}, x{vector.x}, y{vector.y}, z{vector.z} {}

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

        // equal to
        // w * other.w - (Vec3::dot(vector, other.vector)) + w * other.vector + other.w * vector + (Vec3::cross(vector, other.vector)
        tmp.w = (w * other.w) - (x * other.x) - (y * other.y) - (z * other.z);
        tmp.x = (w * other.x) + (x * other.w) + (y * other.z) - (z * other.y);
        tmp.y = (w * other.y) - (x * other.z) + (y * other.w) + (z * other.x);
        tmp.z = (w * other.z) + (x * other.y) - (y * other.x) + (z * other.w);

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
        return (Math::equal(w, other.w) && Math::equal(x, other.x) && Math::equal(y, other.y) && Math::equal(z, other.z));
    }

    bool Quat::operator!=(const Quat& other) const {
        return !(*this == other);
    }

    real& Quat::operator[](unsigned int i) {
        return v[i];
    }

    const real& Quat::operator[](unsigned int i) const {
        return v[i];
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
        // zero quaternion does not have inverse
        if (*this == Quat::zero) {
            throw std::invalid_argument{"Quaternion is zero! Inverse does not exist."};
        }
        // equal to makeConjugate() /= (this->length() * this->length())
        return makeConjugate() /= Quat::dot(*this, *this);
    }

    real Quat::length() const {
        // equal to sqrt(qq*)
        return Math::sqrt(w * w + x * x + y * y + z * z);
    }

    Vec3 Quat::conjugate(const Vec3& v) const {
        Quat q = *this * Quat{0.F, v.x, v.y, v.z} * Quat::inverse(*this);
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
        // ZYX order (degrees)
        return Vec3{Math::pitch(q).get(), Math::yaw(q).get(), Math::roll(q).get()};
    }

    Quat Quat::fromEuler(const Vec3& euler) {
        /*
           from euler ZYX order (radian)

           Qx = [ cos(a/2), sin(a/2) ,    0    ,     0   ]
           Qy = [ cos(b/2),     0    , sin(b/2),     0   ]
           Qz = [ cos(c/2),     0    ,    0    , sin(c/2)]

           Qz * Qy * Qx
        */
        real rzero = real(0.0);
        real divider = real(0.5);
        Quat qx(Math::cos(euler.x * divider), Math::sin(euler.x * divider), rzero, rzero);
        Quat qy(Math::cos(euler.y * divider), rzero, Math::sin(euler.y * divider), rzero);
        Quat qz(Math::cos(euler.z * divider), rzero, rzero, Math::sin(euler.z * divider));

        return qz * qy * qx;
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

    Quat Quat::fromToRotation(const Vec3& from, const Vec3& to) {
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

    Mat4<real> Quat::toRotationMat(const Quat& q) {
        /*
           row-major vector (v)

                    | 1-2y^2-2z^2		2xy+2wz		 2xz-2wy		0 |
                    | 2xy-2wz			1-2x^2-2z^2	 2yz+2wx		0 |
          (x,y,z,0)*| 2xz+2wy			2yz-2wx		 1-2x^2-2y^2	0 |
                    | 0				    0			 0				1 |
        */
        Mat4<real> result;

        static real zero = real(0.0);
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
        result._m44[0][3] = zero;

        result._m44[1][0] = two * (xy + wz);
        result._m44[1][1] = one - two * (xx + zz);
        result._m44[1][2] = two * (yz - wx);
        result._m44[1][3] = zero;

        result._m44[2][0] = two * (xz - wy);
        result._m44[2][1] = two * (yz + wx);
        result._m44[2][2] = one - two * (xx + yy);
        result._m44[2][3] = zero;

        result._m44[3][0] = zero;
        result._m44[3][1] = zero;
        result._m44[3][2] = zero;
        result._m44[3][3] = one;

        return result;
    }

    Quat Quat::fromRotationMat(const Mat4<real>& m) {
        real w = Math::sqrt(m._m44[0][0] + m._m44[1][1] + m._m44[2][2] + 1.0F) * real(0.5);
        real x = (m._m44[2][1] - m._m44[1][2]) / (4 * w);
        real y = (m._m44[0][2] - m._m44[2][0]) / (4 * w);
        real z = (m._m44[1][0] - m._m44[0][1]) / (4 * w);

        return Quat{w, x, y, z};
    }

    real Quat::angleBetween(const Quat& q, const Quat& p) {
//        return Math::acos(Quat::dot(q, p) / (q.length() * p.length()));
        return 0.f;
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

        swap(first.w, second.w);
        swap(first.x, second.x);
        swap(first.y, second.y);
        swap(first.z, second.z);
    }

    const Quat Quat::zero(0.0, 0.0, 0.0, 0.0);
    const Quat Quat::identity(1.0, 0.0, 0.0, 0.0);
}  // namespace GLaDOS
