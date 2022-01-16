#ifndef GLADOS_ANIMATIONCLIP_H
#define GLADOS_ANIMATIONCLIP_H

namespace GLaDOS {
    class AnimationClip {
      public:
        AnimationClip();
        ~AnimationClip();

        AnimationClip(const AnimationClip& other);
        AnimationClip& operator=(const AnimationClip& other);

      private:
    };
}

#endif  // GLADOS_ANIMATIONCLIP_H
