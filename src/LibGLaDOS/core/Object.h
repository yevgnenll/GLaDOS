#ifndef GLADOS_OBJECT_H
#define GLADOS_OBJECT_H

#include <string>

#include "utils/UniqueId.h"
#include "utils/Utility.h"

namespace GLaDOS {
    class Object : public UniqueId {
      public:
        Object() = default;
        virtual ~Object();

        std::string getName() const;
        void setName(const std::string& name);
        bool isActive() const;
        void active(bool value);

        // before being called active (false -> true)
        virtual void onEnable() {}
        // before being called active (true -> false)
        virtual void onDisable() {}
        // when object is destroyed
        virtual void onDestroy() {}

      private:
        virtual void update(real deltaTime) = 0;
        virtual void render() = 0;

      protected:
        std::string mName;
        bool mIsActive{true};
    };
}  // namespace GLaDOS

#endif  //GLADOS_OBJECT_H
