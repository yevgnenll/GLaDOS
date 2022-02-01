#ifndef GLADOS_KEYFRAME_H
#define GLADOS_KEYFRAME_H

#include "utils/Enumeration.h"

namespace GLaDOS {
    template <std::size_t N>
    class KeyFrame {
      public:
        real time;
        real value[N];
        real inTangent[N], outTangent[N];
    };
}

#endif  // GLADOS_KEYFRAME_H
