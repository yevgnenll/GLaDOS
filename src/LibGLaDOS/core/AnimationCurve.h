#ifndef GLADOS_ANIMATIONCURVE_H
#define GLADOS_ANIMATIONCURVE_H

#include "utils/Stl.h"
#include "KeyFrame.h"
#include "utils/Enumeration.h"

namespace GLaDOS {
    class AnimationCurve {
      public:
        explicit AnimationCurve(const Vector<KeyFrame>& keys);
        ~AnimationCurve();

        Vector<KeyFrame> getKeyFrame() const;
        std::size_t length() const;

        int addKey(real time, real value);
        int addKey(const KeyFrame& key);
        void removeKey(int index);
        int moveKey(int index, KeyFrame key);
        real evaluate(real time);

        static AnimationCurve constant(real timeStart, real timeEnd, real value);
        static AnimationCurve easeInOut(real timeStart, real valueStart, real timeEnd, real valueEnd);
        static AnimationCurve linear(real timeStart, real valueStart, real timeEnd, real valueEnd);

      private:
        Vector<KeyFrame> mKeys;
        AnimationWrapMode mPreWrapMode;
        AnimationWrapMode mPostWrapMode;
    };
}

#endif  // GLADOS_ANIMATIONCURVE_H
