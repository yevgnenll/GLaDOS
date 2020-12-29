#include "Math.h"
#include <cmath>

namespace GameEngine {
  float Math::PingPong(float t, float len) {
    float L = 2 * len;
    float T = fmodf(t, L);

    if (0 <= T && T < len) {
      return T;
    }

    return (L - T);
  }
}