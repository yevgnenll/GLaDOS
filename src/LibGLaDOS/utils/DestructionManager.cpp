#include "DestructionManager.h"
#include "Destructor.h"
#include "Utility.h"

#include <algorithm>

namespace GLaDOS {
    void DestructionManager::registerDestructor(Destructor* destructor) {
        mDestructors.emplace_back(destructor);
    }

    void DestructionManager::destroyObjects() {
        // sort the destructor in decreasing order
        std::sort(mDestructors.begin(), mDestructors.end(), std::GreaterPtr<Destructor>());

        for (auto& destructor : mDestructors) {
            destructor->destroy();
        }

        deallocIterable(mDestructors);
    }

    DestructionManager& DestructionManager::getInstance() {
        static DestructionManager instance;
        return instance;
    }
}