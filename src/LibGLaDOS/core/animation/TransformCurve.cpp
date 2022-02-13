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
        if (mTargetBone != nullptr) {
            Transform* boneTransform = mTargetBone->transform();
            if (mTranslation.length() > 1) {
                boneTransform->mLocalPosition = mTranslation.evaluate(time, loop, Interpolation::Linear);
            }
            if (mRotation.length() > 1) {
                boneTransform->mLocalRotation = mRotation.evaluate(time, loop, Interpolation::Cubic);
            }
            if (mScale.length() > 1) {
                boneTransform->mLocalScale = mScale.evaluate(time, loop, Interpolation::Linear);
            }
        }
    }
}