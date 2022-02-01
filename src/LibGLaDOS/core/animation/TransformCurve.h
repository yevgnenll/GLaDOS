#ifndef GLADOS_TRANSFORMCURVE_H
#define GLADOS_TRANSFORMCURVE_H

#include "AnimationCurve.hpp"
#include "core/component/Transform.h"

namespace GLaDOS {
    class TransformCurve {
      public:
        TransformCurve() = default;
        ~TransformCurve() = default;

        real getStartTime() const;
        real getEndTime() const;
        Transform* sample(Transform* targetTransform, real time, bool loop);

        uint32_t mID{0};
        Vec3Curve mTranslation;
        QuatCurve mRotation;
        Vec3Curve mScale;
    };
}

#endif  // GLADOS_TRANSFORMCURVE_H
