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
        real deltaTime() const;
        real deltaTimeUnscaled() const;
        real elapsedTime() const;
        real elapsedTimeUnscaled() const;
        real fixedDeltaTime() const;

        int fps() const;
        void setTimeScale(real value);
        static HighResolutionTimePoint now();
        static real getInterval(HighResolutionTimePoint start, HighResolutionTimePoint end);

      private:
        HighResolutionTimePoint mStart;
        HighResolutionTimePoint mCurrentTime;
        real mFrameAccumulator{0.0};  // for internal use
        real mFixedStepAccumulator{0.0};  // for internal use
        int mFrameCounter{0};  // for internal use
        int mFrameRate{0};
        real mTimeScale{1.0};

        real mDeltaTime{0.0};
        real mUnscaledDeltaTime{0.0};
        real mElapsedTime{0.0};
        real mUnscaledElapsedTime{0.0};
        real mFixedDeltaTime{0.02};
    };
}  // namespace GLaDOS

#endif
