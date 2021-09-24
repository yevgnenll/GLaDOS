#ifndef GLADOS_THREADLOCALSTORAGE_HPP
#define GLADOS_THREADLOCALSTORAGE_HPP

namespace GLaDOS {
    template <typename T>
    class ThreadLocalStorage {
      public:
        static T& get() {
            static thread_local T instance;
            return instance;
        }

        ThreadLocalStorage() = delete;
        ~ThreadLocalStorage() = delete;
    };
}  // namespace GLaDOS

#endif  //GLADOS_THREADLOCALSTORAGE_HPP
