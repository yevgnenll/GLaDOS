#ifndef GLADOS_RESOURCE_H
#define GLADOS_RESOURCE_H

#include <string>

#include "utils/Enumeration.h"
#include "utils/RefCounted.h"
#include "utils/UniqueId.h"

namespace GLaDOS {
  class Resource : public RefCounted, public UniqueId {
  public:
    Resource(ResourceType resourceType);
    virtual ~Resource() = default;

    std::string directory() const;
    void setResourceDir(const std::string& dir);
    std::string name() const;
    void setName(const std::string& name);

  protected:
    std::string mFileDirectory;
    std::string mName;
    ResourceType mResourceType{ResourceType::Undefined};
  };
}  // namespace GLaDOS

#endif
