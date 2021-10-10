#include "Animation.h"
#include "core/AnimationState.h"
#include "core/AnimationClip.h"

namespace GLaDOS {
    Logger* Animation::logger = LoggerRegistry::getInstance().makeAndGetLogger("Animation");

    Animation::Animation() : Component{"Animation"} {

    }

    void Animation::addClip(const AnimationClip& clip, const String& name) {
        // TODO
    }

    void Animation::addClip(const AnimationClip& clip, const String& name, int firstFrame, int lastFrame) {
        // TODO
    }

    void Animation::removeClip(const String& name) {
        // TODO
    }

    bool Animation::isPlaying() const {
        return mIsPlaying;
    }

    bool Animation::playAutomatically() const {
        return mPlayAutomatically;
    }

    void Animation::setPlayAutomatically(bool playAutomatically) {
        mPlayAutomatically = playAutomatically;
    }

    std::size_t Animation::length() const {
        return mAnimations.size();
    }

    AnimationState* Animation::operator [](const String& name) const {
        auto it = mAnimations.find(name);
        if (it == mAnimations.end()) {
            LOG_ERROR(logger, "AnimationState {0} is not exist", String::toUTF8(name).c_str());
            return nullptr;
        }
        return it->second;
    }

    Animation::iterator Animation::begin() {
        return mAnimations.begin();
    }

    Animation::const_iterator Animation::begin() const {
        return mAnimations.cbegin();
    }

    Animation::iterator Animation::end() {
        return mAnimations.end();
    }

    Animation::const_iterator Animation::end() const {
        return mAnimations.cend();
    }

    void Animation::update(real deltaTime) {
    }

    void Animation::render() {
    }
}