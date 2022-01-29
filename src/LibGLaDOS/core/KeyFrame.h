#ifndef GLADOS_KEYFRAME_H
#define GLADOS_KEYFRAME_H

#include <utils/Enumeration.h>

namespace GLaDOS {
    template <std::size_t N>
    class KeyFrame {
      public:
        real time;
        real value[N];
        real inTangent[N], outTangent[N];
    };

    typedef KeyFrame<1> ScalarKeyFrame;
    typedef KeyFrame<2> Vec2KeyFrame;
    typedef KeyFrame<3> Vec3KeyFrame;
    typedef KeyFrame<4> QuatKeyFrame;
}

#endif  // GLADOS_KEYFRAME_H
