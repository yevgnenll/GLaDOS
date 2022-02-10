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
        Mat4<real> offset;
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

    class AssimpLoader {
      public:
        bool loadFromFile(const std::string& fileName, Scene* scene, GameObject* parent);
        Vector<Texture*> getTexture() const;

      private:
        Vector<Mesh*> loadNodeMeshAndMaterial(aiNode* node, const aiScene* aiscene, Scene* scene, GameObject* parent, GameObject* rootBone);
        Mesh* loadMesh(aiMesh* mesh);
        Texture* loadTexture(aiMaterial* material, aiTextureType textureType);
        void buildNodeTable(const aiNode* node, int32_t& boneCounter);
        GameObject* buildBoneHierarchy(const aiNode* node, Scene* scene, GameObject* parent);
        Vector<AnimationClip*> loadAnimation(const aiScene* scene, GameObject* rootNode);
        void getBindPose(Vector<Mat4<real>>& bindPose);

        SceneNode* findNode(const std::string& name);
        int32_t addNode(const SceneNode& node);
        static GameObject* retrieveTargetBone(const std::string& name, GameObject* rootNode);
        static void makeGameObject(const std::string& name, Mesh* mesh, Scene* scene, GameObject* parent, GameObject* rootBone);
        static Mat4<real> toMat4(const aiMatrix4x4& mat);
        static Vec3 toVec3(const aiVector3D& vec3);
        static Vec2 toVec2(const aiVector3D& vec3);

        static Logger* logger;
        static const uint32_t MAX_BONE_INFLUENCE;

        UnorderedMap<std::string, SceneNode> mNodeTable;
        Vector<Texture*> mTextures;
        std::string mDirectoryPath;
    };
}

#endif  // GLADOS_ASSIMPLOADER_H
