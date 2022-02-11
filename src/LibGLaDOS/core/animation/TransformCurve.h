#ifndef GLADOS_TRANSFORMCURVE_H
#define GLADOS_TRANSFORMCURVE_H

#include "AnimationCurve.hpp"

namespace GLaDOS {
    class Transform;
    class GameObject;
    class TransformCurve {
      public:
        TransformCurve() = default;
        ~TransformCurve() = default;

        real getStartTime() const;
        real getEndTime() const;

        void sample(real time, bool loop) const;

        GameObject* mTargetBone{nullptr};
        Vec3Curve mTranslation;
        QuatCurve mRotation;
        Vec3Curve mScale;
    };
}

#endif  // GLADOS_TRANSFORMCURVE_H
