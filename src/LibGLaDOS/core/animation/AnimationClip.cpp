#include "AnimationClip.h"

namespace GLaDOS {
    AnimationClip::AnimationClip(const std::string& name) : mName{name}, mStartTime{0}, mEndTime{0}, mIsLoop{false} {
    }

    void AnimationClip::addCurve(const TransformCurve& curve) {
        mCurves.emplace_back(curve);
    }

    void AnimationClip::sampleAnimation(GameObject* gameObject, real time) const {
        if (Math::equal(getDuration(), real(0))) {
            return;
        }

        time = clampTimeInCurve(time);
        for (uint32_t i = 0; i < length(); i++) {

        }
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
        return mEndTime - mStartTime;
    }

    real AnimationClip::getStartTime() const {
        return mStartTime;
    }

    real AnimationClip::getEndTime() const {
        return mEndTime;
    }

    bool AnimationClip::isLooping() const {
        return mIsLoop;
    }

    void AnimationClip::setLooping(bool loop) {
        mIsLoop = loop;
    }

    real AnimationClip::clampTimeInCurve(real time) const {
        if (mIsLoop) {

        } else {
            time = Math::clamp(time, mStartTime, mEndTime);
        }

        return time;
    }
}