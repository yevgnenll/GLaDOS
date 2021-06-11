#ifndef GLADOS_FIXEDTHREADPOOL_HPP
#define GLADOS_FIXEDTHREADPOOL_HPP

#include <atomic>
#include <functional>
#include <future>
#include <thread>

#include "Utility.h"
#include "memory/FixedSizeMemoryPool.hpp"
#include "utils/ConcurrentQueue.hpp"

namespace GLaDOS {
  // TODO: Must be tested
  using Task = std::function<void()>;
  class FixedThreadPool {
  public:
    FixedThreadPool(uint32_t poolCount = std::thread::hardware_concurrency());
    ~FixedThreadPool();

    template <typename Function, typename... Args>
    void pushTask(const Function&& task, const Args&&... args);
    template <typename Function>
    void pushTask(const Function&& task);
    template <typename Function, typename... Args, typename = std::enable_if_t<std::is_void_v<std::invoke_result_t<std::decay_t<Function>, std::decay_t<Args>...>>>>
    std::future<bool> execute(const Function&& task, Args&&... args);
    template <typename Function, typename... Args, typename R = std::invoke_result_t<std::decay_t<Function>, std::decay_t<Args>...>, typename = std::enable_if_t<!std::is_void_v<R>>>
    std::future<R> execute(const Function&& task, Args&&... args);

    void awaitTermination() const;
    std::size_t getRemainTasksCount() const;
    std::size_t getThreadPoolSize() const;

    DISALLOW_COPY_AND_ASSIGN(FixedThreadPool);

  private:
    void workerThread();
    bool popTask(Task& task);
    static void sleep();

    ConcurrentQueue<Task> mTaskQueue;
    uint32_t mThreadPoolSize{0};
    Vector<std::thread*> mThreadPool;
    std::atomic<bool> mWorkerIsRunning{true};
    FixedSizeMemoryPool<std::thread> mMemoryAllocator;
  };

  template <typename Function, typename... Args>
  void FixedThreadPool::pushTask(const Function&& task, const Args&&... args) {
    pushTask([&task, &args...] { task(std::forward<Args>(args)...); });
  }

  template <typename Function>
  void FixedThreadPool::pushTask(const Function&& task) {
    mTaskQueue.push(std::forward<Task>(task));
  }

  template <typename Function, typename... Args, typename U>
  std::future<bool> FixedThreadPool::execute(const Function&& task, Args&&... args) {
    std::shared_ptr<std::promise<bool>> promise = std::make_shared<std::promise<bool>>();
    std::future<bool> future = promise->get_future();
    pushTask([&task, &args..., promise] {
      task(std::forward<Args>(args)...);
      promise->set_value(true);
    });

    return future;
  }

  template <typename Function, typename... Args, typename R, typename U>
  std::future<R> FixedThreadPool::execute(const Function&& task, Args&&... args) {
    std::shared_ptr<std::promise<R>> promise = std::make_shared<std::promise<R>>();
    std::future<R> future = promise->get_future();
    pushTask([&task, &args..., promise] {
      promise->set_value(task(std::forward<Args>(args)...));
    });

    return future;
  }
}  // namespace GLaDOS

#endif  //GLADOS_FIXEDTHREADPOOL_HPP
