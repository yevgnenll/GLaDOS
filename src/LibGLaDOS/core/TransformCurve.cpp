#include "TransformCurve.h"
#include "math/Math.h"

namespace GLaDOS {
    real TransformCurve::getStartTime() const {
        return Math::min(mTranslation.getStartTime(), mRotation.getStartTime(), mScale.getStartTime());
    }

    real TransformCurve::getEndTime() const {
        return Math::max(mTranslation.getEndTime(), mRotation.getEndTime(), mScale.getEndTime());
    }

    Transform* TransformCurve::sample(Transform* targetTransform, real time, bool loop) {
        targetTransform->mLocalPosition = mTranslation.sample(time, loop);
        targetTransform->mLocalRotation = mRotation.sample(time, loop);
        targetTransform->mLocalScale = mScale.sample(time, loop);
        return targetTransform;
    }
}