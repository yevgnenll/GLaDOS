#include "Resource.h"

namespace GLaDOS {
  Resource::Resource(ResourceType resourceType) : mResourceType{resourceType} {}

  unsigned int Resource::refCount() const {
    return mReferenceCount;
  }

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

  void Resource::AddRef() {
    mReferenceCount++;
  }

  void Resource::RemoveRef() {
    mReferenceCount--;
  }
}  // namespace GLaDOS