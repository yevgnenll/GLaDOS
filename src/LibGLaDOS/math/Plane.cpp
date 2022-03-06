#include "Plane.h"
#include "Math.h"

namespace GLaDOS {
    Plane::Plane(const UVec3 &normal, const Vec3 &pointInPlane) : mNormal{normal} {
        // ax + by + cz + d = 0
        // d = -(ax1 + by1 + cz1)
        // d = -((x1, y1, z1) * (a,b,c))
        mDistance = -Vec3::dot(pointInPlane, normal);
    }

    Plane::Plane(const Vec3& p1, const Vec3& p2, const Vec3& p3) : mNormal{Vec3::cross(p2 - p1, p3 - p1).makeNormalize()} {
        mDistance = -Vec3::dot(p1, mNormal);
    }

    bool Plane::raycast(const Ray &ray, real *enter) {
        float numer = Vec3::dot(ray.getOrigin(), mNormal) + mDistance;
        float denom = Vec3::dot(ray.getDirection(), mNormal);

        // normal is orthogonal to vector, can't intersect
        if (denom < Math::realInfinity) {
            *enter = real(-1);
            return false;
        }
        *enter = -(numer / denom);
        return true;
    }

    UVec3 Plane::getNormal() const {
        return mNormal;
    }

    real Plane::getDistance() const {
        return mDistance;
    }
}