#ifndef GLADOS_CONCURRENTQUEUE_H
#define GLADOS_CONCURRENTQUEUE_H

#include <condition_variable>
#include <mutex>
#include <queue>

#include "Stl.h"

namespace GLaDOS {
    // blocking and non-blocking lock based thread-safe queue implementation
    template <typename T, typename Cont = Deque<T>>
    class ConcurrentQueue {
      public:
        ConcurrentQueue() = default;
        ~ConcurrentQueue() = default;

        void push(const T& item);
        // thread and strong exception safety guarantee
        // wait until queue has at least one element.
        void pop(T& item);
        // return immediately queue element if exists.
        bool tryPop(T& item);
        bool empty() const;
        std::size_t size() const;

      private:
        mutable std::mutex mMutex;
        std::condition_variable mConditionVar;
        Queue<T, Cont> mQueue;
    };

    template <typename T, typename Cont>
    void ConcurrentQueue<T, Cont>::push(const T& item) {
        std::unique_lock<std::mutex> lock(mMutex);
        mQueue.push(item);
        lock.unlock();  // unlock before notification to minimize mutex contention
        mConditionVar.notify_one();  // wake one waiting thread
    }

    template <typename T, typename Cont>
    void ConcurrentQueue<T, Cont>::pop(T& item) {
        std::unique_lock<std::mutex> lock(mMutex);
        while (mQueue.empty()) {
            mConditionVar.wait(lock);
        }
        item = mQueue.front();
        mQueue.pop();
    }

    template <typename T, typename Cont>
    bool ConcurrentQueue<T, Cont>::tryPop(T& item) {
        std::unique_lock<std::mutex> lock(mMutex);
        if (mQueue.empty()) {
            return false;
        }
        item = mQueue.front();
        mQueue.pop();
        return true;
    }

    template <typename T, typename Cont>
    bool ConcurrentQueue<T, Cont>::empty() const {
        std::unique_lock<std::mutex> lock(mMutex);
        return mQueue.empty();
    }

    template <typename T, typename Cont>
    std::size_t ConcurrentQueue<T, Cont>::size() const {
        std::unique_lock<std::mutex> lock(mMutex);
        return mQueue.size();
    }
}  // namespace GLaDOS

#endif  //GLADOS_CONCURRENTQUEUE_H
