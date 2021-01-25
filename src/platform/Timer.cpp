#include "Timer.h"

namespace GameEngine {
  void Timer::reset() {
    mStart = now();
    mTimeScale = 1.0;
  }

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

  real Timer::deltaTime() const { return mDeltaTime; }

  real Timer::deltaTimeUnscaled() const { return mUnscaledDeltaTime; }

  real Timer::elapsedTime() const { return mTime; }

  real Timer::elapsedTimeUnscaled() const { return mUnscaledTime; }

  real Timer::fixedDeltaTime() const { return mFixedDeltaTime; }

  real Timer::fixedDeltaTimeUnscaled() const { return mUnscaledFixedDeltaTime; }

  int Timer::fps() const { return mFrameRate; }

  void Timer::setTimeScale(real value) { mTimeScale = value; }

  HighResolutionTimePoint Timer::now() { return HighResolutionClock::now(); }

  real Timer::getInterval(HighResolutionTimePoint start, HighResolutionTimePoint end) {
    return std::chrono::duration_cast<millisecond>(end - start).count();
  }
}  // namespace GameEngine