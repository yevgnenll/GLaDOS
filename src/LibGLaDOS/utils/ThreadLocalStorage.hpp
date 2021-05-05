#ifndef GLADOS_THREADLOCALSTORAGE_HPP
#define GLADOS_THREADLOCALSTORAGE_HPP

namespace GLaDOS {
  template <typename T>
  class ThreadLocalStore {
  public:
    static T& get() {
      static thread_local T instance;
      return instance;
    }

    ThreadLocalStore() = delete;
    ~ThreadLocalStore() = delete;
  };
}

#endif  //GLADOS_THREADLOCALSTORAGE_HPP
