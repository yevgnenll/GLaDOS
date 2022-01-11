#include "Timer.h"

namespace GLaDOS {
    Timer::Timer() {
        setDestructionPhase(3);
        reset();
    }

    void Timer::update() {
        mCurrentTime = now();

        mUnscaledDeltaTime = getInterval(mStart, mCurrentTime) * 0.001f;
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
        mStart = now();
        mTimeScale = 1.0;
    }

    real Timer::deltaTime() { return mDeltaTime; }

    real Timer::deltaTimeUnscaled() { return mUnscaledDeltaTime; }

    real Timer::elapsedTime() { return mTime; }

    real Timer::elapsedTimeUnscaled() { return mUnscaledTime; }

    real Timer::fixedDeltaTime() { return mFixedDeltaTime; }

    real Timer::fixedDeltaTimeUnscaled() { return mUnscaledFixedDeltaTime; }

    int Timer::fps() { return mFrameRate; }

    void Timer::setTimeScale(real value) { mTimeScale = value; }

    HighResolutionTimePoint Timer::now() { return HighResolutionClock::now(); }

    real Timer::getInterval(HighResolutionTimePoint start, HighResolutionTimePoint end) {
        return std::chrono::duration_cast<millisecond>(end - start).count();
    }
}  // namespace GLaDOS