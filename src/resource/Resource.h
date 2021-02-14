#ifndef GLADOS_RESOURCE_H
#define GLADOS_RESOURCE_H

#include <atomic>
#include <string>
#include <utility>

#include "utils/Enumeration.h"

namespace GLaDOS {
  class Resource {
  public:
    Resource(ResourceType resourceType);
    virtual ~Resource() = default;

    unsigned int getRefCount() const;
    void setResourceDir(const std::string& dir);
    std::string directory() const;

    void AddRef();
    void RemoveRef();

  protected:
    std::atomic<uint32_t> mReferenceCount{0};
    std::string mFileDirectory;
    ResourceType mResourceType{ResourceType::Undefined};
  };
}  // namespace GLaDOS

#endif
