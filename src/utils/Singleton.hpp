#ifndef GAMEENGINE_SINGLETON_HPP
#define GAMEENGINE_SINGLETON_HPP

#include "SpinLock.h"
#include <mutex>

namespace GameEngine {
  template<typename T>
  class Singleton {
  public:
    Singleton() = default;
    virtual ~Singleton() = default;

    static T* getInstance() noexcept;

  private:
    static unsigned char* getBuffer();
    static void destroy();

    static T* instancePointer;
    static SpinLock spinLock;
  };

  template <typename T>
  T* Singleton<T>::instancePointer = nullptr;

  template <typename T>
  SpinLock Singleton<T>::spinLock;

  template<typename T>
  T* Singleton<T>::getInstance() noexcept {
    // classic DCLP implementation
    if (Singleton<T>::instancePointer == nullptr) {
      std::unique_lock<SpinLock> lock{Singleton<T>::spinLock, std::try_to_lock};
      if (Singleton<T>::instancePointer == nullptr) {
        void *buffer = static_cast<void *>(Singleton<T>::getBuffer());
        new (buffer) T{};
        Singleton<T>::instancePointer = reinterpret_cast<T *>(buffer);
        std::atexit(&Singleton<T>::destroy);
      }
    }
    return Singleton<T>::instancePointer;
  }

  template<typename T>
  unsigned char* Singleton<T>::getBuffer() {
    static unsigned char buffer[sizeof(T)];
    return buffer;
  }

  template<typename T>
  void Singleton<T>::destroy() {
    reinterpret_cast<T*>(getBuffer())->~T();
  }
}

#endif  //GAMEENGINE_SINGLETON_HPP
