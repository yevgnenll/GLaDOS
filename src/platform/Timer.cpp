#include "Timer.h"

namespace GLaDOS {
  void Timer::update() {
    mCurrentTime = now();

    mUnscaledDeltaTime = std::chrono::duration_cast<millisecond>(mCurrentTime - mStart).count() * 0.001;
    mDeltaTime = mUnscaledDeltaTime * mTimeScale;

    mUnscaledTime += mUnscaledDeltaTime;
    mTime += mDeltaTime;

    mFixedDeltaTime = mUnscaledFixedDeltaTime * mTimeScale;

    mStart = mCurrentTime;
    mFrameCounter++;
    mAccumulator += mUnscaledDeltaTime;  // accumulate
    if (mAccumulator > real(1.0)) {  // elapsed after 1 sec
      mFrameRate = mFrameCounter;
      mFrameCounter = 0;
      mAccumulator = 0.0;
    }
  }

  void Timer::reset() {
    getInstance()->mStart = now();
    getInstance()->mTimeScale = 1.0;
  }

  real Timer::deltaTime() { return getInstance()->mDeltaTime; }

  real Timer::deltaTimeUnscaled() { return getInstance()->mUnscaledDeltaTime; }

  real Timer::elapsedTime() { return getInstance()->mTime; }

  real Timer::elapsedTimeUnscaled() { return getInstance()->mUnscaledTime; }

  real Timer::fixedDeltaTime() { return getInstance()->mFixedDeltaTime; }

  real Timer::fixedDeltaTimeUnscaled() { return getInstance()->mUnscaledFixedDeltaTime; }

  int Timer::fps() { return getInstance()->mFrameRate; }

  void Timer::setTimeScale(real value) { getInstance()->mTimeScale = value; }

  HighResolutionTimePoint Timer::now() { return HighResolutionClock::now(); }

  real Timer::getInterval(HighResolutionTimePoint start, HighResolutionTimePoint end) {
    return std::chrono::duration_cast<millisecond>(end - start).count();
  }
}  // namespace GLaDOS