#include "Resource.h"

namespace GLaDOS {
    Resource::Resource(ResourceType resourceType) : mResourceType{resourceType} {}

    std::string Resource::directory() const {
        return mFileDirectory;
    }

    void Resource::setResourceDir(const std::string& dir) {
        mFileDirectory = dir;
    }

    std::string Resource::name() const {
        return mName;
    }

    void Resource::setName(const std::string& name) {
        mName = name;
    }

    ResourceType Resource::getType() const {
        return mResourceType;
    }
}  // namespace GLaDOS