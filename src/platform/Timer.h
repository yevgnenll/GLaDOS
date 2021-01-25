#ifndef GAMEENGINE_TIMER_H
#define GAMEENGINE_TIMER_H

#include <chrono>
#include <ctime>
#include <string>

#include "utils/Enumeration.h"
#include "utils/Singleton.hpp"

namespace GameEngine {
  class Timer : public Singleton<Timer> {
  public:
    Timer() = default;

    void reset();
    void update();

    real deltaTime() const;
    real deltaTimeUnscaled() const;
    real elapsedTime() const;
    real elapsedTimeUnscaled() const;
    real fixedDeltaTime() const;
    real fixedDeltaTimeUnscaled() const;

    int fps() const;
    void setTimeScale(real value);
    static HighResolutionTimePoint now();
    static real getInterval(HighResolutionTimePoint start, HighResolutionTimePoint end);

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
}  // namespace GameEngine

#endif
