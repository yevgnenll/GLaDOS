#ifndef GLADOS_KEYFRAME_H
#define GLADOS_KEYFRAME_H

#include <utils/Enumeration.h>

namespace GLaDOS {
    class KeyFrame {
      public:
        KeyFrame(real time, real value) : mTime{time}, mValue{value} {}

        KeyFrame(real time, real value, real inTangent, real outTangent) : KeyFrame{time, value} {
            mInTangent = inTangent;
            mOutTangent = outTangent;
        }

        KeyFrame(real time, real value, real inTangent, real outTangent, real inWeight, real outWeight) : KeyFrame{time, value, inTangent, outTangent} {
            mInWeight = inWeight;
            mOutWeight = outWeight;
        }

      private:
        real mTime;
        real mValue;
        real mInTangent{0}, mOutTangent{0};
        real mInWeight{0}, mOutWeight{0};
    };
}

#endif  // GLADOS_KEYFRAME_H
