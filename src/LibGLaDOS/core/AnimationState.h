#ifndef GLADOS_ANIMATIONSTATE_H
#define GLADOS_ANIMATIONSTATE_H

#include "utils/Enumeration.h"
#include "utils/String.hpp"

namespace GLaDOS {
    class AnimationClip;
    class AnimationState {
      public:
        AnimationState();
        ~AnimationState();

      private:
        AnimationClip* mClip{nullptr};
        String mName;
        real mSpeed{1}; // 1 is normal playback speed
        real mTime{0}; // current time of animation
        AnimationWrapMode mWrapMode;
        bool mEnabled{true};
        AnimationBlendMode mBlendMode{AnimationBlendMode::Blend};
    };
}  // namespace GLaDOS

#endif  //GLADOS_ANIMATIONSTATE_H
