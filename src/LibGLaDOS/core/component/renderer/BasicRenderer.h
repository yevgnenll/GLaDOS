#ifndef GLADOS_BASICRENDERER_H
#define GLADOS_BASICRENDERER_H

#include <core/Component.h>

namespace GLaDOS {
    class Renderable;
    class BasicRenderer : public Component {
      public:
        BasicRenderer()  : Component{"BasicRenderer"} {}
        virtual ~BasicRenderer() {}

        Renderable* getRenderable() { return mRenderable; }

      protected:
        Renderable* mRenderable{nullptr};
    };
}

#endif  // GLADOS_BASICRENDERER_H
