#include "Thread.h"
#include "utils/LoggerRegistry.h"
#include "Utility.h"
#include "memory/Allocation.h"

namespace GLaDOS {
    Logger* Thread::logger = LoggerRegistry::getInstance().makeAndGetLogger("Thread");
    Thread::Thread() : threadId(), mRunnable{nullptr} {
    }

    Thread::Thread(Runnable* runnable) : threadId(), mRunnable{runnable} {
    }

    Thread::~Thread() {
        DELETE_T(mRunnable, Runnable);
    }

    void Thread::start() {
        int ret = pthread_create(&threadId, nullptr, &Thread::mainLoop, this);
        if (ret < 0) {
            LOG_ERROR(logger, "thread create error");
        }
    }

    void Thread::wait() {
        if (pthread_join(threadId, nullptr) != 0) {
            LOG_ERROR(logger, "thread wait error");
        }
    }

    void Thread::cancel() {
        if (pthread_cancel(threadId) != 0) {
            LOG_ERROR(logger, "thread cancel error");
        }
    }

    void Thread::run() {
        if (mRunnable != nullptr) {
            mRunnable->run();
        }
    }

    void* Thread::mainLoop(void* param) {
        auto* threadInstance = static_cast<Thread*>(param);
        threadInstance->run();
    }
}