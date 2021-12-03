#ifndef GLADOS_LINKEDBLOCKINGQUEUE_HPP
#define GLADOS_LINKEDBLOCKINGQUEUE_HPP

#include "Semaphore.h"
#include "Stl.h"

namespace GLaDOS {
    template <typename T>
    class LinkedBlockingQueue {
      public:
        explicit LinkedBlockingQueue(int count);

        void put(const T &t);
        T take();
        int size();

      private:
        Semaphore full;
        Semaphore empty;
        List<T> linkedQueue;
        int queueSize;
    };

    template <typename T>
    LinkedBlockingQueue<T>::LinkedBlockingQueue(int count) : full(), empty(count), linkedQueue{}, queueSize{count} {}

    template <typename T>
    void LinkedBlockingQueue<T>::put(const T &t) {
        empty.wait();
        linkedQueue.push_back(t);
        full.notify();
    }

    template <typename T>
    T LinkedBlockingQueue<T>::take() {
        full.wait();
        T result = linkedQueue.front();
        linkedQueue.pop_front();
        empty.notify();
        return result;
    }

    template <typename T>
    int LinkedBlockingQueue<T>::size() {
        return queueSize;
    }
}

#endif  // GLADOS_LINKEDBLOCKINGQUEUE_HPP
