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
struct aiAnimation;
template<typename TReal>
class aiMatrix4x4t;
typedef float ai_real;
typedef aiMatrix4x4t<ai_real> aiMatrix4x4;

namespace GLaDOS {
    struct BoneInfo {
        int32_t id;
        std::string name;
        Mat4<real> offset;
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

    class Texture;
    class Mesh;
    class Material;
    class Renderable;
    class AnimationClip;
    class Logger;
    class AssimpLoader {
      public:
        bool loadFromFile(const std::string& filePath);
        Vector<Mesh*> getMesh() const;
        Vector<Texture*> getTexture() const;

      private:
        bool loadNode(aiNode* node, const aiScene* scene);
        Mesh* loadMesh(aiMesh* mesh);
        void parseBoneWeight(Vector<Vertex>& vertices, aiBone* bone);
        int32_t findOrCacheBone(const std::string& name, aiBone* bone);
        Texture* loadTexture(aiMaterial* material, aiTextureType textureType);
        AnimationClip* loadAnimation(aiAnimation* animation);

        static Mat4<real> toMat4(const aiMatrix4x4& mat);
        static Vec3 toVec3(const aiVector3D& vec3);
        static Vec2 toVec2(const aiVector3D& vec3);

        static Logger* logger;
        static const uint32_t MAX_BONE_INFLUENCE;

        UnorderedMap<std::string, BoneInfo*> mBoneMap;
        Vector<Mesh*> mMeshes;
        Vector<Texture*> mTextures;
        Vector<AnimationClip*> mAnimationClips;
        std::string mDirectoryPath;
        int32_t mNumBoneCount{0};
    };
}

#endif  // GLADOS_ASSIMPLOADER_H
