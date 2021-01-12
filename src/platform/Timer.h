#ifndef GAMEENGINE_TIMER_H
#define GAMEENGINE_TIMER_H

#include <chrono>
#include <ctime>
#include <string>

#include "utils/Enumeration.h"

namespace GameEngine {
  class Timer {
  public:
    Timer();

    static void reset();
    void update();

    static float deltaTime();
    static float deltaTimeUnscaled();
    static float elapsedTime();
    static float elapsedTimeUnscaled();
    static float fixedDeltaTime();
    static float fixedDeltaTimeUnscaled();

    static int fps();
    static void setTimeScale(float value);
    static HighResolutionTimePoint now();
    static float getInterval(HighResolutionTimePoint start, HighResolutionTimePoint end);

  private:
    static Timer* instance;

    HighResolutionTimePoint mStart;
    HighResolutionTimePoint mCurrentTime;
    float mAccumulator{0.f};  // for internal use
    int mFrameCounter{0};  // for internal use
    int mFrameRate{0};
    float mTimeScale{1.f};

    float mDeltaTime{0.f};
    float mUnscaledDeltaTime{0.f};
    float mTime{0.f};
    float mUnscaledTime{0.f};
    float mFixedDeltaTime{0.f};
    float mUnscaledFixedDeltaTime{0.02f};
  };
}  // namespace GameEngine

#endif
