#include "FixedThreadPool.hpp"

#include <chrono>

namespace GLaDOS {
  FixedThreadPool::FixedThreadPool(const uint32_t poolCount) : mThreadPoolSize{poolCount != 0 ? poolCount : std::thread::hardware_concurrency()}, mMemoryAllocator{mThreadPoolSize} {
    mThreadPool.reserve(mThreadPoolSize);
    for (uint32_t i = 0; i < mThreadPoolSize; i++) {
      mThreadPool.emplace_back(mMemoryAllocator.allocate(&FixedThreadPool::workerThread, this));
    }
  }

  FixedThreadPool::~FixedThreadPool() {
    awaitTermination();
    mWorkerIsRunning = false;
    for (auto& thread : mThreadPool) {
      thread->join();
    }
    mThreadPool.clear();
  }

  void FixedThreadPool::awaitTermination() const {
    while (!mTaskQueue.empty()) {
      FixedThreadPool::sleep();
    }
  }

  std::size_t FixedThreadPool::getRemainTasksCount() const {
    return mTaskQueue.size();
  }

  std::size_t FixedThreadPool::getThreadPoolSize() const {
    return mThreadPoolSize;
  }

  void FixedThreadPool::workerThread() {
    Task task;
    while (mWorkerIsRunning) {
      if (popTask(task)) {
        task();
        continue;
      }
      FixedThreadPool::sleep();
    }
  }

  bool FixedThreadPool::popTask(Task& task) {
    return mTaskQueue.tryPop(task);
  }

  void FixedThreadPool::sleep() {
    static const uint32_t sleepDuration = 1000;
    std::this_thread::sleep_for(std::chrono::microseconds(sleepDuration));
  }
}  // namespace GLaDOS