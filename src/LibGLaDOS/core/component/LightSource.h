#ifndef GLADOS_LIGHTSOURCE_H
#define GLADOS_LIGHTSOURCE_H

#include "core/Component.h"

namespace GLaDOS {
    class LightSource : public Component {
      public:
        LightSource();
        ~LightSource();

      protected:
        void fixedUpdate(real fixedDeltaTime) override;
        void update(real deltaTime) override;
        void render() override;
        Component* clone() override;

      private:
    };
}  // namespace GLaDOS

#endif  // GLADOS_LIGHTSOURCE_H
