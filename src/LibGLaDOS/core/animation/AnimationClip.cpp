#include "AnimationClip.h"

namespace GLaDOS {
    AnimationClip::AnimationClip(const std::string& name) : mName{name}, mStartTime{0}, mEndTime{0}, mIsLoop{false} {
    }

    void AnimationClip::addCurve(const TransformCurve& curve) {
        mCurves.emplace_back(curve);
    }

    std::string AnimationClip::getName() const {
        return mName;
    }

    void AnimationClip::setName(const std::string& name) {
        mName = name;
    }

    std::size_t AnimationClip::length() const {
        return mCurves.size();
    }

    real AnimationClip::getDuration() const {
        return 0;
    }

    real AnimationClip::getStartTime() const {
        return 0;
    }

    real AnimationClip::getEndTime() const {
        return 0;
    }

    bool AnimationClip::isLooping() const {
        return mIsLoop;
    }

    void AnimationClip::setLooping(bool loop) {
        mIsLoop = loop;
    }

    void AnimationClip::sampleAnimation(GameObject* gameObject, real time) const {
        if (Math::equal(getDuration(), real(0))) {
            return;
        }


    }
}