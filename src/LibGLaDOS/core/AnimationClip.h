#ifndef GLADOS_ANIMATIONCLIP_H
#define GLADOS_ANIMATIONCLIP_H

#include <string>
#include "utils/Enumeration.h"
#include "utils/Stl.h"
#include "TransformCurve.h"

namespace GLaDOS {
    class AnimationClip {
      public:
        AnimationClip(const std::string& name);
        ~AnimationClip() = default;

        std::string getName() const;
        std::size_t length() const;
        real getDuration() const;
        real getStartTime() const;
        real getEndTime() const;
        bool isLooping() const;
        void setLooping(bool loop);

      private:
        std::string mName;
        real mStartTime;
        real mEndTime;
        bool isLoop;
        Vector<TransformCurve> mCurves;
    };
}

#endif  // GLADOS_ANIMATIONCLIP_H
