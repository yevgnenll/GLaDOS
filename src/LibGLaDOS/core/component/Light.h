#ifndef GLADOS_LIGHT_H
#define GLADOS_LIGHT_H

#include "core/Component.h"

namespace GLaDOS {
    class Light : public Component {
      public:
        Light();
        ~Light();

      protected:
        void fixedUpdate(real fixedDeltaTime) override;
        void update(real deltaTime) override;
        void render() override;
        Component* clone() override;

      private:
    };
}  // namespace GLaDOS

#endif  // GLADOS_LIGHT_H
