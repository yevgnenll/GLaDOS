#ifndef GAMEENGINE_TIMER_H
#define GAMEENGINE_TIMER_H

#include <chrono>
#include <ctime>
#include <string>

#include "utils/Singleton.hpp"
#include "utils/Enumeration.h"

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
    real mAccumulator{0.f};  // for internal use
    int mFrameCounter{0};  // for internal use
    int mFrameRate{0};
    real mTimeScale{1.f};

    real mDeltaTime{0.f};
    real mUnscaledDeltaTime{0.f};
    real mTime{0.f};
    real mUnscaledTime{0.f};
    real mFixedDeltaTime{0.f};
    real mUnscaledFixedDeltaTime{0.02f};
  };
}  // namespace GameEngine

#endif
