#ifndef GLADOS_SEMAPHORE_H
#define GLADOS_SEMAPHORE_H

#include <mutex>

namespace GLaDOS {
  class Semaphore {
  public:
    explicit Semaphore(int _count = 0);

    void notify();
    void wait();

  private:
    std::mutex mtx;
    std::condition_variable cv;
    int count;
  };
}  // namespace GLaDOS

#endif  //GLADOS_SEMAPHORE_H
