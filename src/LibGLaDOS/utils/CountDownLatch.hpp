#ifndef GLADOS_COUNTDOWNLATCH_HPP
#define GLADOS_COUNTDOWNLATCH_HPP

#include <chrono>
#include <condition_variable>
#include <mutex>

#include "Utility.h"

namespace GLaDOS {
    class CountDownLatch {
      public:
        explicit CountDownLatch(uint32_t count);
        virtual ~CountDownLatch() = default;

        void await();
        template <typename Rep, typename Period>
        bool await(const std::chrono::duration<Rep, Period>& timeout);
        void countDown();
        uint32_t getCount();

        DISALLOW_COPY_AND_ASSIGN(CountDownLatch);

      private:
        std::mutex mMutex;
        std::condition_variable mConditionVariable;
        uint32_t mCountDown{0};
    };

    template <typename Rep, typename Period>
    bool CountDownLatch::await(const std::chrono::duration<Rep, Period>& timeout) {
        std::unique_lock<std::mutex> lock(mMutex);
        bool result = true;
        if (mCountDown > 0) {
            result = mConditionVariable.wait_for(lock, timeout, [this]() { return mCountDown == 0; });
        }

        return result;
    }
}  // namespace GLaDOS

#endif  //GLADOS_COUNTDOWNLATCH_HPP
