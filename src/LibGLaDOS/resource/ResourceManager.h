#ifndef GLADOS_RESOURCEMANAGER_H
#define GLADOS_RESOURCEMANAGER_H

#include "utils/Singleton.hpp"
#include "utils/Utility.h"
#include "Resource.h"

namespace GLaDOS {
    class ResourceManager : public Singleton<ResourceManager> {
      public:
        ResourceManager();
        ~ResourceManager() override;

        ResourceManager(const ResourceManager &) = delete;
        ResourceManager &operator=(const ResourceManager &) = delete;

        bool remove(Resource* resource);
        bool store(Resource* resource);
        Resource* getResource(const std::string& name);

      private:
        static Logger* logger;
        Map<std::string, Resource*> mResources;
    };
}  // namespace GLaDOS

#endif  //GLADOS_RESOURCEMANAGER_H
