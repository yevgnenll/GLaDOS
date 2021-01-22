#ifndef GAMEENGINE_RESOURCE_H
#define GAMEENGINE_RESOURCE_H

#include <atomic>
#include <string>
#include <utility>

#include "utils/Enumeration.h"

namespace GameEngine {
  class Resource {
  public:
    Resource() = default;
    virtual ~Resource() = default;

    unsigned int getReferenceCount() const;
    void setResourcePath(const std::string& path);
    std::string getBasePath() const;
    std::string getFileName() const;
    std::string getFullFilePath() const;

    void AddReference();
    void RemoveReference();

  protected:
    std::atomic<unsigned int> mReferenceCount{0};
    std::string mBasePath{};
    std::string mFileName{};
    ResourceType mResourceType{ResourceType::Undefined};
  };
}  // namespace GameEngine

#endif
