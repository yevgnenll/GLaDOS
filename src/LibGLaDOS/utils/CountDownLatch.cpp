#include "CountDownLatch.hpp"

namespace GLaDOS {
  CountDownLatch::CountDownLatch(uint32_t count) : mCountDown{count} {}

  void CountDownLatch::await() {
    std::unique_lock<std::mutex> lock(mMutex);
    if (mCountDown > 0) {
      mConditionVariable.wait(lock, [this]() { return mCountDown == 0; });
    }
  }

  void CountDownLatch::countDown() {
    std::unique_lock<std::mutex> lock(mMutex);
    if (mCountDown > 0) {
      mCountDown--;
      mConditionVariable.notify_all();
    }
  }

  uint32_t CountDownLatch::getCount() {
    std::unique_lock<std::mutex> lock(mMutex);
    return mCountDown;
  }
}  // namespace GLaDOS