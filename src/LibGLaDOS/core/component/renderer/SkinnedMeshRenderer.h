#ifndef GLADOS_SKINNEDMESHRENDERER_H
#define GLADOS_SKINNEDMESHRENDERER_H

#include "MeshRenderer.h"

namespace GLaDOS {
    class SkinnedMeshRenderer : public MeshRenderer {
      public:
        SkinnedMeshRenderer();
        ~SkinnedMeshRenderer() override;
    };
}  // namespace GLaDOS

#endif  //GLADOS_SKINNEDMESHRENDERER_H
