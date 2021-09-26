#ifndef GLADOS_ANIMATION_H
#define GLADOS_ANIMATION_H

#include "core/Component.h"
#include "utils/String.hpp"

namespace GLaDOS {
    class AnimationState;
    class AnimationClip;
    class Animation : public Component {
        typedef Map<String, AnimationState*>::iterator iterator;
        typedef Map<String, AnimationState*>::const_iterator const_iterator;

      public:
        Animation();
        ~Animation() override = default;

        void addClip(const AnimationClip& clip, const String& name);
        void addClip(const AnimationClip& clip, const String& name, int firstFrame, int lastFrame);
        void removeClip(const String& name);

        bool isPlaying() const;
        bool playAutomatically() const;
        void setPlayAutomatically(bool playAutomatically);

        std::size_t length() const;
        AnimationState* operator [](const String& name) const;
        iterator begin();
        const_iterator begin() const;
        iterator end();
        const_iterator end() const;

      protected:
        void update(real deltaTime) override;
        void render() override;

      private:
        static Logger* logger;

        Map<String, AnimationState*> mAnimations;
        AnimationWrapMode mWrapMode{AnimationWrapMode::Loop}; // default wrapMode for all animationState if not specified
        bool mIsPlaying{false};
        bool mPlayAutomatically{true};
    };
}

#endif  // GLADOS_ANIMATION_H
