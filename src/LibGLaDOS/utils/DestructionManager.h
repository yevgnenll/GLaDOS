#ifndef GLADOS_DESTRUCTIONMANAGER_H
#define GLADOS_DESTRUCTIONMANAGER_H

#include "Stl.h"

namespace GLaDOS {
    // http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.21.8456&rep=rep1&type=pdf
    class Destructor;
    class DestructionManager {
      public:
        DestructionManager() = default;
        ~DestructionManager() = default;

        void registerDestructor(Destructor* destructor);
        void destroyObjects();

        static DestructionManager& getInstance();

      private:
        Vector<Destructor*> mDestructors;
    };
}

#endif  // GLADOS_DESTRUCTIONMANAGER_H
