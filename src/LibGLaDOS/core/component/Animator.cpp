#include "Animator.h"
#include "core/animation/AnimationState.h"
#include "core/animation/AnimationClip.h"

namespace GLaDOS {
    Logger* Animator::logger = LoggerRegistry::getInstance().makeAndGetLogger("Animator");

    Animator::Animator() : Component{"Animator"}, mCurrentState{mAnimations.end()} {
    }

    Animator::~Animator() {
        deallocValueInMap(mAnimations);
    }

    void Animator::play(const std::string& name) {
        auto animationState = mAnimations.find(name);
        if (animationState == mAnimations.end()) {
            LOG_ERROR(logger, "AnimationState `{0}` is not exist", name);
            return;
        }
        if (mCurrentState != animationState) {
            mCurrentState = animationState;
            mCurrentState->second->mCurrentTime = real(0);
        }
    }

    void Animator::rewind(const std::string& name) {
    }

    void Animator::stop(const std::string& name) {
    }

    void Animator::addClip(AnimationClip* clip, const std::string& name) {
        auto iter = mAnimations.find(name);
        if (iter != mAnimations.end()) {
            LOG_ERROR(logger, "AnimationState `{0}` already exist", name);
            return;
        }
        AnimationState* newState = NEW_T(AnimationState);
        newState->setName(name);
        newState->setClip(clip);
        newState->setTicksPerSecond(clip->getInitialTicksPerSecond());
        mAnimations.insert(std::make_pair(name, newState));
    }

    bool Animator::removeClip(const std::string& name) {
        auto iter = mAnimations.find(name);
        if (iter == mAnimations.end()) {
            return false;
        }
        mAnimations.erase(iter);
        return true;
    }

    AnimationState* Animator::getCurrentState() {
        return mCurrentState->second;
    }

    void Animator::getClipNames(Vector<std::string>& clips) const {
        clips.clear();
        std::transform(mAnimations.begin(), mAnimations.end(), std::back_inserter(clips),
                       [](const auto& clip) { return clip.first; });
    }

    bool Animator::isPlaying() const {
        return mCurrentState != mAnimations.end();
    }

    std::size_t Animator::length() const {
        return mAnimations.size();
    }

    void Animator::fixedUpdate(real fixedDeltaTime) {
        // Nothing to do here
    }

    void Animator::update(real deltaTime) {
        if (mCurrentState == mAnimations.end()) {
            return;
        }
        mCurrentState->second->update(deltaTime);
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
        animator->mCurrentState = mCurrentState;
        return animator;
    }
}