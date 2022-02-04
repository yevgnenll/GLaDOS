#ifndef GLADOS_ASSIMPLOADER_H
#define GLADOS_ASSIMPLOADER_H

#include <string>
#include <assimp/material.h>
#include "utils/Stl.h"
#include "math/Mat4.hpp"
#include "platform/render/VertexFormat.h"

struct aiNode;
struct aiScene;
struct aiMesh;
struct aiBone;
struct aiMaterial;
template<typename TReal>
class aiMatrix4x4t;
typedef float ai_real;
typedef aiMatrix4x4t<ai_real> aiMatrix4x4;

namespace GLaDOS {
    class AnimationClip;
    class Scene;
    class GameObject;
    class Texture;
    class Mesh;
    class Logger;

    struct SceneNode {
        int32_t id;
        std::string name;
        bool isBone;
    };
    struct Vertex {
        Vec3 position;
        Vec3 normal;
        Vec3 tangent;
        Vec3 biTangent;
        Vec4 boneWeight;
        int32_t boneIndex[4];
        Vec2 texcoord;
    };
    struct Animation {
        real duration{0};
        real ticksPerSecond{1};
        AnimationClip* clip;
    };

    class AssimpLoader {
      public:
        bool loadFromFile(const std::string& fileName, Scene* scene, GameObject* parent);
        Vector<Texture*> getTexture() const;
        Vector<Animation*> getAnimation() const;

      private:
        void loadNodeMeshAndMaterial(aiNode* node, const aiScene* aiscene, Scene* scene, GameObject* parent, GameObject* rootBone);
        Mesh* loadMesh(aiMesh* mesh);
        void loadBoneWeight(Vector<Vertex>& vertices, aiBone* bone);
        Texture* loadTexture(aiMaterial* material, aiTextureType textureType);
        void buildNodeTable(const aiNode* node);
        GameObject* buildBoneHierarchy(const aiNode* node, Scene* scene, GameObject* parent);
        void loadAnimation(const aiScene* scene);

        SceneNode* findNode(const std::string& name);
        int32_t addNode(const SceneNode& node);
        static void makeGameObject(const std::string& name, Mesh* mesh, Scene* scene, GameObject* parent, GameObject* rootBone);
        static Mat4<real> toMat4(const aiMatrix4x4& mat);
        static Vec3 toVec3(const aiVector3D& vec3);
        static Vec2 toVec2(const aiVector3D& vec3);

        static Logger* logger;
        static const uint32_t MAX_BONE_INFLUENCE;

        UnorderedMap<std::string, SceneNode> mNodeTable;
        Vector<Texture*> mTextures;
        Vector<Animation*> mAnimations;
        std::string mDirectoryPath;
        int32_t mNumNodes{0};
    };
}

#endif  // GLADOS_ASSIMPLOADER_H
