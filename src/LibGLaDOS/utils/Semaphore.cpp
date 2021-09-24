#include "Semaphore.h"

namespace GLaDOS {
    Semaphore::Semaphore(uint32_t count) : mCount{count} {}

    void Semaphore::notify() {
        std::unique_lock<std::mutex> lock(mMutex);
        mCount++;
        //notify the waiting thread
        mConditionVariable.notify_one();
    }

    void Semaphore::wait() {
        std::unique_lock<std::mutex> lock(mMutex);
        while (mCount == 0) {
            //wait on the mutex until notify is called
            mConditionVariable.wait(lock);
        }
        mCount--;
    }
}  // namespace GLaDOS