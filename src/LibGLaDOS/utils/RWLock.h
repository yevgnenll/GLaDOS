#ifndef GLADOS_RWLOCK_H
#define GLADOS_RWLOCK_H

#include <shared_mutex>

namespace GLaDOS {
    // TODO
    class RWLock {
      public:
        RWLock();
        ~RWLock();

        void readLock();
        void readUnlock();
        void writeLock();
        void writeUnLock();

      private:
        std::shared_timed_mutex mSharedMutex;
    };
}  // namespace GLaDOS

#endif  //GLADOS_RWLOCK_H
