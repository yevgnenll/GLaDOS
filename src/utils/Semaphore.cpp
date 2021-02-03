#include "Semaphore.h"

namespace GLaDOS {
  Semaphore::Semaphore(int _count) : count{_count} {}

  void Semaphore::notify() {
    std::unique_lock<std::mutex> lock(mtx);
    count++;
    //notify the waiting thread
    cv.notify_one();
  }

  void Semaphore::wait() {
    std::unique_lock<std::mutex> lock(mtx);
    while (count == 0) {
      //wait on the mutex until notify is called
      cv.wait(lock);
    }
    count--;
  }
}  // namespace GLaDOS