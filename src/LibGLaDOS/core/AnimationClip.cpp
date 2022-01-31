#include "AnimationClip.h"

namespace GLaDOS {
    AnimationClip::AnimationClip(const std::string& name) {
    }

    std::string AnimationClip::getName() const {
        return std::string();
    }

    std::size_t AnimationClip::length() const {
        return 0;
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
        return false;
    }

    void AnimationClip::setLooping(bool loop) {
    }
}