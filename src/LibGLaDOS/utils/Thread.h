#ifndef GLADOS_THREAD_H
#define GLADOS_THREAD_H

#include <pthread.h>
#include <cstddef>

namespace GLaDOS {
    struct Runnable {
        virtual ~Runnable() = default;
        virtual void run() = 0;
    };

    class Logger;
    class Thread : public Runnable {
      public:
        Thread();
        explicit Thread(Runnable* runnable);
        ~Thread() override;

        void start();
        void wait();
        void cancel();
        void run() override;

      private:
        static Logger* logger;

        pthread_t threadId;
        Runnable* mRunnable;

        static void* mainLoop(void* param);
    };
}

#endif  // GLADOS_THREAD_H
