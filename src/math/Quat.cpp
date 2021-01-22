#include "Quat.h"

#include "Vec3.h"

namespace GameEngine {
  Quat::Quat(float _w, float _x, float _y, float _z) : w{_w}, x{_x}, y{_y}, z{_z} {}

  Quat::Quat(float _w, const Vec3& _v) : w(_w), x(_v.x), y(_v.y), z(_v.z) {}

  Quat& Quat::operator=(Quat other) {
    // copy and swap idiom (effective c++ section 11)
    Quat::swap(*this, other);
    return *this;
  }

  void Quat::swap(Quat& first, Quat& second) {
    using std::swap;

    swap(first.x, second.x);
    swap(first.y, second.y);
    swap(first.z, second.z);
    swap(first.w, second.w);
  }
}  // namespace GameEngine
