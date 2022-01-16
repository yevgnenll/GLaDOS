#ifndef GLADOS_MESHRENDERER_H
#define GLADOS_MESHRENDERER_H

#include "core/component/renderer/BasicRenderer.h"

namespace GLaDOS {
    class Logger;
    class Mesh;
    class Material;
    class MeshRenderer : public BasicRenderer {
      public:
        MeshRenderer();
        MeshRenderer(Mesh* mesh, Material* material);
        ~MeshRenderer() override;

      private:
        static Logger* logger;

      protected:
        void update(real deltaTime) override;
        void render() override;
        Component* clone() override;
    };
}  // namespace GLaDOS

#endif  //GLADOS_MESHRENDERER_H