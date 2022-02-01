#include "Animator.h"
#include "core/animation/AnimationState.h"
#include "core/animation/AnimationClip.h"

namespace GLaDOS {
    Logger* Animator::logger = LoggerRegistry::getInstance().makeAndGetLogger("Animator");

    Animator::Animator() : Component{"Animator"} {

    }

    void Animator::play(const std::string& name) {
    }

    void Animator::rewind(const std::string& name) {
    }

    void Animator::stop(const std::string& name) {
    }

    std::size_t Animator::getClipCount() const {
        return 0;
    }

    void Animator::addClip(const AnimationClip& clip, const std::string& name) {
        // TODO
    }

    void Animator::addClip(const AnimationClip& clip, const std::string& name, int firstFrame, int lastFrame) {
        // TODO
    }

    void Animator::removeClip(const std::string& name) {
        // TODO
    }

    bool Animator::isPlaying() const {
        return mIsPlaying;
    }

    bool Animator::playAutomatically() const {
        return mPlayAutomatically;
    }

    void Animator::setPlayAutomatically(bool playAutomatically) {
        mPlayAutomatically = playAutomatically;
    }

    std::size_t Animator::length() const {
        return mAnimations.size();
    }

    AnimationState* Animator::operator [](const std::string& name) const {
        auto animationState = mAnimations.find(name);
        if (animationState == mAnimations.end()) {
            LOG_ERROR(logger, "AnimationState `{0}` is not exist", name);
            return nullptr;
        }
        return animationState->second;
    }

    Animator::iterator Animator::begin() {
        return mAnimations.begin();
    }

    Animator::const_iterator Animator::begin() const {
        return mAnimations.cbegin();
    }

    Animator::iterator Animator::end() {
        return mAnimations.end();
    }

    Animator::const_iterator Animator::end() const {
        return mAnimations.cend();
    }

    void Animator::fixedUpdate(real fixedDeltaTime) {
    }

    void Animator::update(real deltaTime) {
    }

    void Animator::render() {
        // Nothing to do here
    }

    Component* Animator::clone() {
        Animator* animator = NEW_T(Animator);
        animator->mIsActive = mIsActive;
        for (const auto& pair : mAnimations) {
            animator->mAnimations.insert(std::make_pair(pair.first, NEW_T(AnimationState(*pair.second))));
        }
        animator->mWrapMode = mWrapMode;
        animator->mIsPlaying = mIsPlaying;
        animator->mPlayAutomatically = mPlayAutomatically;
        return animator;
    }
}