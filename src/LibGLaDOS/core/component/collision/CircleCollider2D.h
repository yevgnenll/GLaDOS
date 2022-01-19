#ifndef GLADOS_CIRCLECOLLIDER2D_H
#define GLADOS_CIRCLECOLLIDER2D_H

#include "Collider.h"
#include "math/Vec2.h"

namespace GLaDOS {
    class CircleCollider2D : public Collider {
      public:
        CircleCollider2D(Vec2 center, real radius);
        ~CircleCollider2D() override;

      private:
        Vec2 mCenter{0.f, 0.f};
        real mRadius{1.f};
    };
}

#endif  // GLADOS_CIRCLECOLLIDER2D_H
