#include "ResourceManager.h"

namespace GLaDOS {
    Logger* ResourceManager::logger = LoggerRegistry::getInstance().makeAndGetLogger("ResourceManager");

    ResourceManager::ResourceManager() {
        this->setDestructionPhase(1);
    }

    ResourceManager::~ResourceManager() {
        deallocValueInMap(mResources);
    }

    bool ResourceManager::remove(Resource* resource) {
        if (resource == nullptr) {
            LOG_TRACE(logger, "resource is null");
            return false;
        }

        auto iter = mResources.find(resource->name());
        if(iter != mResources.end()) {
            mResources.erase(iter);
            LOG_TRACE(logger, "Resource {0} removed", resource->name());
            return true;
        }
        return false;
    }

    bool ResourceManager::store(Resource* resource) {
        if (resource == nullptr) {
            LOG_TRACE(logger, "resource is null");
            return false;
        }

        auto result = mResources.insert(std::make_pair(resource->name(), resource));
        if (result.second) {
            LOG_TRACE(logger, "Resource {0} newly added", resource->name());
        }
        return result.second;
    }

    Resource* ResourceManager::getResource(const String& name) {
        auto iter = mResources.find(name);
        if (iter != mResources.end()) {
            return iter->second;
        }
        return nullptr;
    }
}