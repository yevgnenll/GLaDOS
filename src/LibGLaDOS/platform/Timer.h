#ifndef GLADOS_TIMER_H
#define GLADOS_TIMER_H

#include <chrono>
#include <ctime>
#include <string>

#include "utils/Enumeration.h"
#include "utils/Singleton.hpp"

namespace GLaDOS {
    class Timer : public Singleton<Timer> {
      public:
        Timer();

        void update();

        void reset();
        real deltaTime();
        real deltaTimeUnscaled();
        real elapsedTime();
        real elapsedTimeUnscaled();
        real fixedDeltaTime();
        real fixedDeltaTimeUnscaled();

        int fps();
        void setTimeScale(real value);
        HighResolutionTimePoint now();
        real getInterval(HighResolutionTimePoint start, HighResolutionTimePoint end);

      private:
        HighResolutionTimePoint mStart;
        HighResolutionTimePoint mCurrentTime;
        real mAccumulator{0.0};  // for internal use
        int mFrameCounter{0};  // for internal use
        int mFrameRate{0};
        real mTimeScale{1.0};

        real mDeltaTime{0.0};
        real mUnscaledDeltaTime{0.0};
        real mTime{0.0};
        real mUnscaledTime{0.0};
        real mFixedDeltaTime{0.0};
        real mUnscaledFixedDeltaTime{0.02};
    };
}  // namespace GLaDOS

#endif
