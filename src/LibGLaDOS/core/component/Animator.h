#ifndef GLADOS_ANIMATOR_H
#define GLADOS_ANIMATOR_H

#include <string>

#include "core/Component.h"

namespace GLaDOS {
    class AnimationState;
    class AnimationClip;
    class Animator : public Component {
      public:
        Animator();
        ~Animator() override;

        void play(const std::string& name);
        void rewind(const std::string& name);
        void stop(const std::string& name);

        void addClip(AnimationClip* clip, const std::string& name);
        bool removeClip(const std::string& name);

        void getClipNames(Vector<std::string>& clips) const;
        bool isPlaying() const;
        std::size_t length() const;

      protected:
        void fixedUpdate(real fixedDeltaTime) override;
        void update(real deltaTime) override;
        void render() override;
        Component* clone() override;

      private:
        static Logger* logger;

        UnorderedMap<std::string, AnimationState*> mAnimations;
        UnorderedMap<std::string, AnimationState*>::const_iterator mCurrentState;
    };
}

#endif  // GLADOS_ANIMATOR_H
