#ifndef GLADOS_SKINNEDMESHRENDERER_H
#define GLADOS_SKINNEDMESHRENDERER_H

#include "MeshRenderer.h"

namespace GLaDOS {
    class GameObject;
    class SkinnedMeshRenderer : public MeshRenderer {
      public:
        SkinnedMeshRenderer();
        SkinnedMeshRenderer(Mesh* mesh, Material* material, GameObject* rootBone);
        ~SkinnedMeshRenderer() override;

        void setRootBone(GameObject* gameObject);

      protected:
        void update(real deltaTime) override;
        void render() override;
        Component* clone() override;

      private:
        static Logger* logger;

        GameObject* mRootBone;
    };
}  // namespace GLaDOS

#endif  //GLADOS_SKINNEDMESHRENDERER_H
