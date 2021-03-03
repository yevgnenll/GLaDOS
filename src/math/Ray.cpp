#include "Ray.h"

namespace GLaDOS {
  Ray::Ray(const Vec3& origin, const Vec3& direction) : mOrigin{origin}, mDirection{direction}, mInvDirection{Vec3::inverse(direction)} {
  }

  Ray::Ray(Ray&& other) : Ray{} {
    Ray::swap(*this, other);
  }

  Ray& Ray::operator=(Ray other) {
    // copy and swap idiom (effective c++ section 11)
    Ray::swap(*this, other);
    return *this;
  }

  Vec3 Ray::point(float distance) {
    return mOrigin + mDirection * distance;
  }

  bool Ray::operator==(const Ray& other) {
    return ((mOrigin == other.mOrigin) && (mDirection == other.mDirection) && mInvDirection == other.mInvDirection);
  }

  bool Ray::operator!=(const Ray& other) {
    return !(*this == other);
  }

  void Ray::swap(Ray& first, Ray& second) {
    using std::swap;

    swap(first.mOrigin, second.mOrigin);
    swap(first.mDirection, second.mDirection);
    swap(first.mInvDirection, second.mInvDirection);
  }
}