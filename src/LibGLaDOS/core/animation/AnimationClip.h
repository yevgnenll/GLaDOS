#ifndef GLADOS_ANIMATIONCLIP_H
#define GLADOS_ANIMATIONCLIP_H

#include <string>
#include "utils/Enumeration.h"
#include "utils/Stl.h"
#include "TransformCurve.h"

namespace GLaDOS {
    class GameObject;
    class AnimationClip {
      public:
        AnimationClip(const std::string& name);
        ~AnimationClip() = default;

        void addCurve(const TransformCurve& curve);
        std::string getName() const;
        void setName(const std::string& name);
        std::size_t length() const;
        real getDuration() const;
        real getStartTime() const;
        real getEndTime() const;
        bool isLooping() const;
        void setLooping(bool loop);
        void sampleAnimation(GameObject* gameObject, real time) const;

      private:
        std::string mName;
        real mStartTime;
        real mEndTime;
        bool mIsLoop;
        Vector<TransformCurve> mCurves;
    };
}

#endif  // GLADOS_ANIMATIONCLIP_H
