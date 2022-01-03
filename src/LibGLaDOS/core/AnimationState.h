#ifndef GLADOS_ANIMATIONSTATE_H
#define GLADOS_ANIMATIONSTATE_H

#include <string>

#include "utils/Enumeration.h"

namespace GLaDOS {
    class AnimationClip;
    class AnimationState {
      public:
        AnimationState();
        ~AnimationState();

      private:
        AnimationClip* mClip{nullptr};
        std::string mName;
        real mSpeed{1}; // 1 is normal playback speed
        real mTime{0}; // current time of animation
        AnimationWrapMode mWrapMode;
        bool mEnabled{true};
        AnimationBlendMode mBlendMode{AnimationBlendMode::Blend};
    };
}  // namespace GLaDOS

#endif  //GLADOS_ANIMATIONSTATE_H
