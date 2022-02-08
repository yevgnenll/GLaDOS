#ifndef GLADOS_SKINNEDMESHRENDERER_H
#define GLADOS_SKINNEDMESHRENDERER_H

#include "MeshRenderer.h"
#include "utils/Stl.h"
#include "math/Mat4.hpp"

namespace GLaDOS {
    class Mesh;
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
        static constexpr std::size_t MAX_BONE_MATRIX = 96;

        void buildMatrixPalette(GameObject* node, Mesh* mesh, const Mat4<real>& parentMatrix, std::size_t& matrixIndex);

        GameObject* mRootBone;
        Vector<Mat4<real>> mMatrixPalette{MAX_BONE_MATRIX};
    };
}  // namespace GLaDOS

#endif  //GLADOS_SKINNEDMESHRENDERER_H
