#include "TransformCurve.h"
#include "core/GameObject.hpp"
#include "core/component/Transform.h"
#include "math/Math.h"

namespace GLaDOS {
    real TransformCurve::getStartTime() const {
        return Math::min(mTranslation.getStartTime(), mRotation.getStartTime(), mScale.getStartTime());
    }

    real TransformCurve::getEndTime() const {
        return Math::max(mTranslation.getEndTime(), mRotation.getEndTime(), mScale.getEndTime());
    }

    void TransformCurve::sample(real time, bool loop) const {
        mTargetBone->transform()->mLocalPosition = mTranslation.evaluate(time, loop);
        mTargetBone->transform()->mLocalRotation = mRotation.evaluate(time, loop);
        mTargetBone->transform()->mLocalScale = mScale.evaluate(time, loop);
    }
}