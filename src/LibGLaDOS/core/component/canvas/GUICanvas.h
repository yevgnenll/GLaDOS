#ifndef GLADOS_GUICANVAS_H
#define GLADOS_GUICANVAS_H

#include "core/Component.h"

namespace GLaDOS {
    class AbstractFrame;
    class GUICanvas : public Component {
      public:
        GUICanvas();
        ~GUICanvas() override;

        void addFrame(AbstractFrame* frame);

      protected:
        void fixedUpdate(real fixedDeltaTime) override;
        void update(real deltaTime) override;
        void render() override;
        Component* clone() override;

      private:
    };
}

#endif  // GLADOS_GUICANVAS_H
