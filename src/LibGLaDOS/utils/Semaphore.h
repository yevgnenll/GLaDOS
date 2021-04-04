#ifndef GLADOS_SEMAPHORE_H
#define GLADOS_SEMAPHORE_H

#include <mutex>

#include "Utility.h"

namespace GLaDOS {
  class Semaphore {
  public:
    explicit Semaphore(uint32_t count = 0);

    void notify();
    void wait();

    DISALLOW_COPY_AND_ASSIGN(Semaphore);

  private:
    std::mutex mMutex;
    std::condition_variable mConditionVariable;
    uint32_t mCount;
  };
}  // namespace GLaDOS

#endif  //GLADOS_SEMAPHORE_H
