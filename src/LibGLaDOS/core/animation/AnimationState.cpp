#include "AnimationState.h"
#include "AnimationClip.h"

namespace GLaDOS {
    AnimationState::~AnimationState() {
        DELETE_T(mClip, AnimationClip);
    }

    AnimationState::AnimationState(const AnimationState& other)
        : mClip{other.mClip}, mSpeed{other.mSpeed}, mTime{other.mTime},
          mWrapMode{other.mWrapMode}, mBlendMode{other.mBlendMode} {
        mName = other.mName;
    }

    AnimationClip* AnimationState::getClip() const {
        return mClip;
    }

    void AnimationState::setClip(AnimationClip* clip) {
        mClip = clip;
    }

    real AnimationState::getSpeed() const {
        return mSpeed;
    }

    void AnimationState::setSpeed(real speed) {
        mSpeed = speed;
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
            mTime = mClip->sampleAnimation(mTime + (deltaTime * mSpeed));
        }
    }

    void AnimationState::render() {
    }

    AnimationState& AnimationState::operator=(const AnimationState& other) {
        mClip = other.mClip;
        mName = other.mName;
        mSpeed = other.mSpeed;
        mTime = other.mTime;
        mWrapMode = other.mWrapMode;
        mBlendMode = other.mBlendMode;
        return *this;
    }
}