#ifndef GLADOS_MESHRENDERER_H
#define GLADOS_MESHRENDERER_H

#include "core/Component.h"

namespace GLaDOS {
    class Logger;
    class Mesh;
    class Material;
    class Renderable;
    class MeshRenderer : public Component {
      public:
        MeshRenderer();
        MeshRenderer(Mesh* mesh, Material* material);
        ~MeshRenderer() override;

        void setRenderable(Renderable* renderable);

      private:
        static Logger* logger;

      protected:
        void update(real deltaTime) override;
        void render() override;

        Renderable* mRenderable{nullptr};
    };
}  // namespace GLaDOS

#endif  //GLADOS_MESHRENDERER_H