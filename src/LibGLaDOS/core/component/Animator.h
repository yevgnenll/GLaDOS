#ifndef GLADOS_ANIMATOR_H
#define GLADOS_ANIMATOR_H

#include <string>

#include "core/Component.h"

namespace GLaDOS {
    class AnimationState;
    class AnimationClip;
    class Animator : public Component {
        typedef Map<std::string, AnimationState*>::iterator iterator;
        typedef Map<std::string, AnimationState*>::const_iterator const_iterator;

      public:
        Animator();
        ~Animator() override = default;

        void play(const std::string& name);
        void rewind(const std::string& name);
        void stop(const std::string& name);
        std::size_t getClipCount() const;

        void addClip(const AnimationClip& clip, const std::string& name);
        void addClip(const AnimationClip& clip, const std::string& name, int firstFrame, int lastFrame);
        void removeClip(const std::string& name);

        bool isPlaying() const;
        bool playAutomatically() const;
        void setPlayAutomatically(bool playAutomatically);

        std::size_t length() const;
        AnimationState* operator [](const std::string& name) const;
        Animator::iterator begin();
        Animator::const_iterator begin() const;
        Animator::iterator end();
        Animator::const_iterator end() const;

      protected:
        void fixedUpdate(real fixedDeltaTime) override;
        void update(real deltaTime) override;
        void render() override;
        Component* clone() override;

      private:
        static Logger* logger;

        Map<std::string, AnimationState*> mAnimations;
        AnimationWrapMode mWrapMode{AnimationWrapMode::Loop}; // default wrapMode for all animationState if not specified
        bool mIsPlaying{false};
        bool mPlayAutomatically{true};
    };
}

#endif  // GLADOS_ANIMATOR_H
