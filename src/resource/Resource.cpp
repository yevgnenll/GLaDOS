#include "Resource.h"

#include "utils/StringUtils.h"

namespace GameEngine {
  unsigned int Resource::getReferenceCount() const {
    return mReferenceCount;
  }

  void Resource::setResourcePath(const std::string& path) {
    auto [baseName, fileName] = StringUtils::extractBaseFileNamePair(path);
    mBasePath = baseName;
    mFileName = fileName;
  }

  std::string Resource::getBasePath() const {
    return mBasePath;
  }

  std::string Resource::getFileName() const {
    return mFileName;
  }

  std::string Resource::getFullFilePath() const {
    return mBasePath + mFileName;
  }

  void Resource::AddReference() {
    mReferenceCount++;
  }

  void Resource::RemoveReference() {
    mReferenceCount--;
  }
}  // namespace GameEngine