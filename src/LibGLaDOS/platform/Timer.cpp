#include "Timer.h"
#include "core/SceneManager.h"

namespace GLaDOS {
    Timer::Timer() {
        setDestructionPhase(3);
        reset();
    }

    void Timer::update() {
        mCurrentTime = now();
        mUnscaledDeltaTime = getInterval(mStart, mCurrentTime);
        mStart = mCurrentTime;

        mFixedStepAccumulator += mUnscaledDeltaTime;
        if (mFixedStepAccumulator >= mFixedDeltaTime) {
            SceneManager::getInstance().fixedUpdate(mFixedDeltaTime);
            mFixedStepAccumulator -= mFixedDeltaTime;
        }

        mDeltaTime = mUnscaledDeltaTime * mTimeScale;
        mElapsedTime += mDeltaTime;
        mUnscaledElapsedTime += mUnscaledDeltaTime;

        mFrameCounter++;
        mFrameAccumulator += mUnscaledDeltaTime;
        if (mFrameAccumulator > real(1.0)) {  // elapsed after 1 sec
            mFrameRate = mFrameCounter;
            mFrameCounter = 0;
            mFrameAccumulator = 0.0;
        }
    }

    void Timer::reset() {
        mStart = now();
        mTimeScale = 1.0;
    }

    real Timer::deltaTime() const { return mDeltaTime; }

    real Timer::deltaTimeUnscaled() const { return mUnscaledDeltaTime; }

    real Timer::elapsedTime() const { return mElapsedTime; }

    real Timer::elapsedTimeUnscaled() const { return mUnscaledElapsedTime; }

    real Timer::fixedDeltaTime() const { return mFixedDeltaTime; }

    int Timer::fps() const { return mFrameRate; }

    void Timer::setTimeScale(real value) { mTimeScale = value; }

    HighResolutionTimePoint Timer::now() { return HighResolutionClock::now(); }

    real Timer::getInterval(HighResolutionTimePoint start, HighResolutionTimePoint end) {
        return std::chrono::duration<real>(end - start).count();
    }
}  // namespace GLaDOS