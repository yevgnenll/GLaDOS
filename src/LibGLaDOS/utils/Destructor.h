#ifndef GLADOS_DESTRUCTOR_H
#define GLADOS_DESTRUCTOR_H

namespace GLaDOS {
    class DestructionPhase {
      public:
        explicit DestructionPhase(unsigned int phase) : mPhase{phase} {}

        bool operator >(const DestructionPhase& rhs) const {
            return mPhase > rhs.mPhase;
        }

        unsigned int mPhase;
    };

    class Destructor {
      public:
        Destructor(DestructionPhase destructionPhase) : mDestructionPhase{destructionPhase} {}
        virtual ~Destructor() = default;

        bool operator >(const Destructor& rhs) const {
            return mDestructionPhase > rhs.mDestructionPhase;
        }

        virtual void destroy() = 0;

      private:
        DestructionPhase mDestructionPhase;
    };

    template <typename T>
    class TDestructor : public Destructor {
      public:
        TDestructor(T* object, DestructionPhase destructionPhase) : Destructor{destructionPhase}, mObjectPointer{object} {}
        ~TDestructor() override = default;
        void destroy() override {
            DELETE_T(mObjectPointer, T);
        }

      private:
        T* mObjectPointer;
    };
}

#endif  // GLADOS_DESTRUCTOR_H
