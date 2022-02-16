#ifndef GLADOS_BONERENDERER_H
#define GLADOS_BONERENDERER_H

#include "MeshRenderer.h"

namespace GLaDOS {
    class BoneRenderer : public MeshRenderer {
      public:
        BoneRenderer();
        ~BoneRenderer() override;

      protected:
        void render() override;
        void update(real deltaTime) override;
        Component* clone() override;
    };
}

#endif  // GLADOS_BONERENDERER_H
