#include "AnimationState.h"
#include "AnimationClip.h"

namespace GLaDOS {
    AnimationState::~AnimationState() {
        DELETE_T(mClip, AnimationClip);
    }

    AnimationState::AnimationState(const AnimationState& other)
        : mClip{other.mClip}, mTicksPerSecond{other.mTicksPerSecond}, mCurrentTime{other.mCurrentTime}, mWrapMode{other.mWrapMode}, mBlendMode{other.mBlendMode} {
        mName = other.mName;
    }

    AnimationClip* AnimationState::getClip() const {
        return mClip;
    }

    void AnimationState::setClip(AnimationClip* clip) {
        mClip = clip;
    }

    real AnimationState::getTicksPerSecond() const {
        return mTicksPerSecond;
    }

    void AnimationState::setTicksPerSecond(real speed) {
        mTicksPerSecond = speed;
    }

    AnimationWrapMode AnimationState::getWrapMode() const {
        return mWrapMode;
    }

    void AnimationState::setWrapMode(AnimationWrapMode wrapMode) {
        mWrapMode = wrapMode;
    }

    void AnimationState::fixedUpdate(real fixedDeltaTime) {
    }

    void AnimationState::update(real deltaTime) {
        if (mIsActive) {
            mCurrentTime = mClip->sampleAnimation(mCurrentTime + (deltaTime * mTicksPerSecond));
        }
    }

    void AnimationState::render() {
    }

    AnimationState& AnimationState::operator=(const AnimationState& other) {
        mClip = other.mClip;
        mName = other.mName;
        mTicksPerSecond = other.mTicksPerSecond;
        mCurrentTime = other.mCurrentTime;
        mWrapMode = other.mWrapMode;
        mBlendMode = other.mBlendMode;
        return *this;
    }
}