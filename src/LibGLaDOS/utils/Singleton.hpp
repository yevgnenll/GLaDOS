#ifndef GLADOS_SINGLETON_HPP
#define GLADOS_SINGLETON_HPP

#include <memory>
#include <mutex>

#include "memory/Allocation.h"

namespace GLaDOS {
  // https://silviuardelean.ro/2012/06/05/few-singleton-approaches/
  template <typename T>
  class Singleton {
  public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static T& getInstance() noexcept;

  protected:
    Singleton() = default;
    virtual ~Singleton() = default;

  private:
    static void destroy(T* ptr);

    static std::shared_ptr<T> instancePointer;
    static std::once_flag onlyOne;
  };

  template <typename T>
  std::shared_ptr<T> Singleton<T>::instancePointer = nullptr;
  template <typename T>
  std::once_flag Singleton<T>::onlyOne;

  template <typename T>
  T& Singleton<T>::getInstance() noexcept {
    std::call_once(onlyOne, []() {
      instancePointer.reset(NEW_T(T{}), Singleton<T>::destroy);
    });
    return *instancePointer;
  }

  template <typename T>
  void Singleton<T>::destroy(T* ptr) {
    DELETE_T(ptr, T);
  }
}  // namespace GLaDOS

#endif  //GLADOS_SINGLETON_HPP
