#include "Resource.h"

namespace GLaDOS {
  Resource::Resource(ResourceType resourceType) : mResourceType{resourceType} {}

  unsigned int Resource::getRefCount() const {
    return mReferenceCount;
  }

  void Resource::setResourceDir(const std::string& dir) {
    mFileDirectory = dir;
  }

  std::string Resource::directory() const {
    return mFileDirectory;
  }

  void Resource::AddRef() {
    mReferenceCount++;
  }

  void Resource::RemoveRef() {
    mReferenceCount--;
  }
}  // namespace GLaDOS