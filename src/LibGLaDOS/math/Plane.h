#ifndef GLADOS_PLANE_H
#define GLADOS_PLANE_H

#include "Vec3.h"
#include "UVec3.h"
#include "Ray.h"

namespace GLaDOS {
    // plane equation
    // dot((x - a), n) = 0
    // ax + by + cz + d = 0
    class Plane {
      public:
        Plane(const UVec3& normal, const Vec3& pointInPlane);
        Plane(const Vec3& p1, const Vec3& p2, const Vec3& p3);

        bool raycast(const Ray& ray, real* enter);
        UVec3 getNormal() const;
        real getDistance() const;

      private:
        UVec3 mNormal;
        real mDistance;
    };
}  // namespace GLaDOS

#endif  //GLADOS_PLANE_H
