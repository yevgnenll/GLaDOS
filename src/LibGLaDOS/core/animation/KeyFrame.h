#ifndef GLADOS_KEYFRAME_H
#define GLADOS_KEYFRAME_H

#include "utils/Enumeration.h"

namespace GLaDOS {
    template <std::size_t N>
    class KeyFrame {
      public:
        KeyFrame(real _time, real _value[N]);

        real time;
        real value[N];
        real inTangent[N], outTangent[N];
    };

    template <std::size_t N>
    KeyFrame<N>::KeyFrame(real _time, real *_value) : time{_time} {
        std::copy(_value, _value + N, value);
        std::fill(inTangent, inTangent + N, 0);
        std::fill(outTangent, outTangent + N, 0);
    }
}

#endif  // GLADOS_KEYFRAME_H
