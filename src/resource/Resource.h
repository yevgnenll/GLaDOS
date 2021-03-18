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

    unsigned int refCount() const;
    std::string directory() const;
    void setResourceDir(const std::string& dir);
    std::string name() const;
    void setName(const std::string& name);

  protected:
    void AddRef();
    void RemoveRef();

    std::atomic<uint32_t> mReferenceCount{0};
    std::string mFileDirectory;
    std::string mName;
    ResourceType mResourceType{ResourceType::Undefined};
  };
}  // namespace GLaDOS

#endif
