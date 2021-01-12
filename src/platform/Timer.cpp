#include "Timer.h"

namespace GameEngine {
  Timer* Timer::instance = nullptr;

  Timer::Timer() {
    instance = this;
  }

  void Timer::reset() {
    instance->mStart = now();
    instance->mTimeScale = 1.f;
  }

  void Timer::update() {
    mCurrentTime = now();

    mUnscaledDeltaTime = std::chrono::duration_cast<millisecond>(mCurrentTime - mStart).count() * 0.001f;
    mDeltaTime = mUnscaledDeltaTime * mTimeScale;

    mUnscaledTime += mUnscaledDeltaTime;
    mTime += mDeltaTime;

    mFixedDeltaTime = mUnscaledFixedDeltaTime * mTimeScale;

    mStart = mCurrentTime;
    mFrameCounter++;
    mAccumulator += mUnscaledDeltaTime;  // accumulate
    if (mAccumulator > 1.0f) {  // elapsed after 1 sec
      mFrameRate = mFrameCounter;
      mFrameCounter = 0;
      mAccumulator = 0.0f;
    }
  }

  float Timer::deltaTime() { return instance->mDeltaTime; }

  float Timer::deltaTimeUnscaled() { return instance->mUnscaledDeltaTime; }

  float Timer::elapsedTime() { return instance->mTime; }

  float Timer::elapsedTimeUnscaled() { return instance->mUnscaledTime; }

  float Timer::fixedDeltaTime() { return instance->mFixedDeltaTime; }

  float Timer::fixedDeltaTimeUnscaled() { return instance->mUnscaledFixedDeltaTime; }

  int Timer::fps() { return instance->mFrameRate; }

  void Timer::setTimeScale(float value) { instance->mTimeScale = value; }

  HighResolutionTimePoint Timer::now() { return HighResolutionClock::now(); }

  float Timer::getInterval(HighResolutionTimePoint start, HighResolutionTimePoint end) {
    return std::chrono::duration_cast<millisecond>(end - start).count();
  }
}  // namespace GameEngine