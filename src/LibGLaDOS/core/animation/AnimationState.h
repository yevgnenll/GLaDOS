#ifndef GLADOS_ANIMATIONSTATE_H
#define GLADOS_ANIMATIONSTATE_H

#include <string>
#include "core/Object.h"

#include "utils/Enumeration.h"

namespace GLaDOS {
    class AnimationClip;
    class AnimationState : public Object {
        friend class Animator;
      public:
        AnimationState() = default;
        ~AnimationState() override;

        AnimationState(const AnimationState& other);
        AnimationState& operator=(const AnimationState& other);

        AnimationClip* getClip() const;
        void setClip(AnimationClip* clip);
        real getSpeed() const;
        void setSpeed(real speed);
        AnimationWrapMode getWrapMode() const;
        void setWrapMode(AnimationWrapMode wrapMode);

      protected:
        void fixedUpdate(real fixedDeltaTime) override;
        void update(real deltaTime) override;
        void render() override;

      private:
        AnimationClip* mClip{nullptr};
        real mSpeed{1}; // 1 is normal playback speed
        real mTime{0}; // current time of animation
        AnimationWrapMode mWrapMode;
        AnimationBlendMode mBlendMode{AnimationBlendMode::Blend};
    };
}  // namespace GLaDOS

#endif  //GLADOS_ANIMATIONSTATE_H
