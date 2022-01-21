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
        Quat q = *this * Quat{0.F, other.x, other.y, other.z} * Quat::inverse(*this);
        return Vec3{q.x, q.y, q.z};
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
        // zero quaternion does not have inverse (the only case)
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
           from euler ZYX order (degree)

           Qx = [ cos(a/2), sin(a/2) ,    0    ,     0   ]
           Qy = [ cos(b/2),     0    , sin(b/2),     0   ]
           Qz = [ cos(c/2),     0    ,    0    , sin(c/2)]

           Qz * Qy * Qx
        */
        real rzero = real(0.0);
        real divider = real(0.5);
        Vec3 angleInRad = Math::toRadians(euler);

        Quat qx(Math::cos(angleInRad.x * divider), Math::sin(angleInRad.x * divider), rzero, rzero);
        Quat qy(Math::cos(angleInRad.y * divider), rzero, Math::sin(angleInRad.y * divider), rzero);
        Quat qz(Math::cos(angleInRad.z * divider), rzero, rzero, Math::sin(angleInRad.z * divider));

        return qz * qy * qx;
    }

    Quat Quat::angleAxis(Deg angle, const UVec3& axis) {
        auto halfAngle = Math::toRadians(angle) * real(0.5);
        auto halfSin = Math::sin(halfAngle.get());

        return Quat{
            Math::cos(halfAngle.get()),
            halfSin * axis->x,
            halfSin * axis->y,
            halfSin * axis->z};
    }

    Quat Quat::fromRotation(const Mat4<real>& m) {
        real w = Math::sqrt(m._m44[0][0] + m._m44[1][1] + m._m44[2][2] + 1.0F) * real(0.5);
        real x = (m._m44[2][1] - m._m44[1][2]) / (4 * w);
        real y = (m._m44[0][2] - m._m44[2][0]) / (4 * w);
        real z = (m._m44[1][0] - m._m44[0][1]) / (4 * w);

        return Quat{w, x, y, z};
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

    Deg Quat::angleBetween(const Quat& q, const Quat& p) {
        // calculate the shortest angle between two Quaternion.
        real d = Math::min(Math::abs(Quat::dot(q, p)), (real)1.f);
        return Math::toDegrees(Rad{2.f * Math::acos(d)});
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
