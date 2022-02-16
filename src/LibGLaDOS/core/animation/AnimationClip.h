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

        void addCurve(const TransformCurve& curve);
        real sampleAnimation(real time) const;

        std::string getName() const;
        void setName(const std::string& name);
        std::size_t length() const;
        real getDuration() const;
        real getStartTime() const;
        void setStartTime(real startTime);
        real getEndTime() const;
        void setEndTime(real endTime);

        bool isLooping() const;
        void setLooping(bool loop);
        real getInitialTicksPerSecond() const;
        void setInitialTicksPerSecond(real ticksPerSecond);

      private:
        real clampTimeInCurve(real time) const;

        Vector<TransformCurve> mCurves;
        std::string mName;
        real mStartTime;
        real mEndTime;
        real mInitialTicksPerSecond;
        bool mIsLoop;
    };
}

#endif  // GLADOS_ANIMATIONCLIP_H
