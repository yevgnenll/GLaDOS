#include "RWLock.h"

namespace GLaDOS {
    RWLock::RWLock() {
    }

    RWLock::~RWLock() {
    }

    void RWLock::readLock() {
        mSharedMutex.lock_shared();
    }

    void RWLock::readUnlock() {
        mSharedMutex.unlock_shared();
    }

    void RWLock::writeLock() {
        mSharedMutex.lock();
    }

    void RWLock::writeUnLock() {
        mSharedMutex.unlock();
    }
}  // namespace GLaDOS