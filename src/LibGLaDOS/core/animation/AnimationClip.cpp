#include "AnimationClip.h"
#include "core/GameObject.hpp"
#include "core/component/Transform.h"

namespace GLaDOS {
    AnimationClip::AnimationClip(const std::string& name) : mName{name}, mStartTime{0}, mEndTime{0}, mIsLoop{false} {
    }

    void AnimationClip::addCurve(const TransformCurve& curve) {
        mCurves.emplace_back(curve);
    }

    real AnimationClip::sampleAnimation(real time) const {
        if (Math::equal(getDuration(), real(0))) {
            return real(0);
        }

        time = clampTimeInCurve(time);
        for (const TransformCurve& curve : mCurves) {
            curve.sample(time, mIsLoop);
        }

        return time;
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

    void AnimationClip::setStartTime(real startTime) {
        mStartTime = startTime;
    }

    real AnimationClip::getEndTime() const {
        return mEndTime;
    }

    void AnimationClip::setEndTime(real endTime) {
        mEndTime = endTime;
    }

    bool AnimationClip::isLooping() const {
        return mIsLoop;
    }

    void AnimationClip::setLooping(bool loop) {
        mIsLoop = loop;
    }

    real AnimationClip::clampTimeInCurve(real time) const {
        if (mIsLoop) {
            real duration = getDuration();
            if (getDuration() <= real(0)) {
                return real(0);
            }

            time = Math::mod(time - mStartTime, duration);
            if (time < real(0)) {
                time += duration;
            }
            time = time + mStartTime;
        } else {
            time = Math::clamp(time, mStartTime, mEndTime);
        }

        return time;
    }
}