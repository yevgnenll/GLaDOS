#ifndef GLADOS_SINGLETON_HPP
#define GLADOS_SINGLETON_HPP

#include <memory>
#include <mutex>

#include "memory/Allocation.h"
#include "Destructor.h"
#include "DestructionManager.h"

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

        DestructionPhase mDestructionPhase{0};

        void setDestructionPhase(unsigned int phase);

      private:
        static T* instancePointer;
        static std::once_flag onlyOne;
    };

    template <typename T>
    T* Singleton<T>::instancePointer = nullptr;
    template <typename T>
    std::once_flag Singleton<T>::onlyOne;

    template <typename T>
    T& Singleton<T>::getInstance() noexcept {
        std::call_once(onlyOne, []() {
            instancePointer = NEW_T(T{});
            DestructionManager::getInstance().registerDestructor(NEW_T(TDestructor<T>(instancePointer, instancePointer->mDestructionPhase)));
        });
        return *instancePointer;
    }

    template <typename T>
    void Singleton<T>::setDestructionPhase(unsigned int phase) {
        this->mDestructionPhase.mPhase = phase;
    }
}  // namespace GLaDOS

#endif  //GLADOS_SINGLETON_HPP
