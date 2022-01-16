#include "AnimationState.h"
#include "AnimationClip.h"

namespace GLaDOS {
    AnimationState::AnimationState() {

    }

    AnimationState::~AnimationState() {

    }

    AnimationState::AnimationState(const AnimationState& other)
        : mClip{other.mClip}, mName{other.mName}, mSpeed{other.mSpeed}, mTime{other.mTime},
          mWrapMode{other.mWrapMode}, mEnabled{other.mEnabled}, mBlendMode{other.mBlendMode} {
    }

    AnimationState& AnimationState::operator=(const AnimationState& other) {
        mClip = other.mClip;
        mName = other.mName;
        mSpeed = other.mSpeed;
        mTime = other.mTime;
        mWrapMode = other.mWrapMode;
        mEnabled = other.mEnabled;
        mBlendMode = other.mBlendMode;
        return *this;
    }
}