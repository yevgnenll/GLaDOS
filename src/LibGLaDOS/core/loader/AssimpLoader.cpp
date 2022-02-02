#include "AssimpLoader.h"
#include "utils/LoggerRegistry.h"
#include "utils/Utility.h"
#include "utils/StringUtils.h"
#include "math/Vec3.h"
#include "platform/render/IndexBuffer.h"
#include "platform/render/Material.h"
#include "platform/render/Mesh.h"
#include "platform/Platform.h"
#include "platform/render/Renderer.h"
#include "platform/render/VertexBuffer.h"
#include "platform/render/Texture2D.h"
#include "core/animation/TransformCurve.h"
#include "core/animation/AnimationClip.h"
#include "RootDir.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace GLaDOS {
    Logger* AssimpLoader::logger = LoggerRegistry::getInstance().makeAndGetLogger("AssimpLoader");
    const uint32_t AssimpLoader::MAX_BONE_INFLUENCE = 4;

    bool AssimpLoader::loadFromFile(const std::string& filePath) {
        mDirectoryPath = StringUtils::splitFileName(filePath).first;
        std::string fileDirectory = std::string(RESOURCE_DIR) + filePath;

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(fileDirectory, aiProcessPreset_TargetRealtime_Quality);
        if (scene == nullptr || ((scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0u) || scene->mRootNode == nullptr) {
            LOG_ERROR(logger, "Load from file error: {0}", importer.GetErrorString());
            return false;
        }

        // load node mesh and material
        loadNode(scene->mRootNode, scene);

        // load bone hierarchy
        loadBone(mRootBone, scene->mRootNode);

        // load animations
        loadAnimation(scene);

        return true;
    }

    Vector<Mesh*> AssimpLoader::getMesh() const {
        return mMeshes;
    }

    Vector<Texture*> AssimpLoader::getTexture() const {
        return mTextures;
    }

    Vector<Animation*> AssimpLoader::getAnimation() const {
        return mAnimations;
    }

    Bone* AssimpLoader::getBone() {
        return &mRootBone;
    }

    void AssimpLoader::loadNode(aiNode* node, const aiScene* scene) {
        static const Array<aiTextureType, 4> textureTypes = { aiTextureType_DIFFUSE, aiTextureType_SPECULAR, aiTextureType_AMBIENT, aiTextureType_NORMALS };

        // load mesh at the current node
        for (uint32_t i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            Mesh* currentMesh = loadMesh(mesh);
            if (currentMesh != nullptr) {
                mMeshes.emplace_back(currentMesh);
            }

            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            for (const auto textureType : textureTypes) {
                if (material->GetTextureCount(textureType) > 0) {
                    Texture* texture = loadTexture(material, textureType);
                    if (texture != nullptr) {
                        mTextures.emplace_back(texture);
                    }
                }
            }
        }

        // recursively load all the child node
        for (uint32_t j = 0; j < node->mNumChildren; j++) {
            loadNode(node->mChildren[j], scene);
        }
    }

    Mesh* AssimpLoader::loadMesh(aiMesh* mesh) {
        VertexFormatDescriptor vertexDesc = VertexFormatDescriptor().position().normal().tangent().biTangent().boneWeight().boneIndex().texCoord0();

        // process mesh's vertices
        Vector<Vertex> vertices(mesh->mNumVertices);
        for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;
            vertex.position = toVec3(mesh->mVertices[i]);

            if (mesh->HasNormals()) {
                vertex.normal = toVec3(mesh->mNormals[i]);
            }

            if (mesh->HasTangentsAndBitangents()) {
                vertex.tangent = toVec3(mesh->mTangents[i]);
                vertex.biTangent = toVec3(mesh->mBitangents[i]);
            }

            if (mesh->HasTextureCoords(0)) {
                vertex.texcoord = toVec2(mesh->mTextureCoords[0][i]);
            }

            for (uint32_t j = 0; j < MAX_BONE_INFLUENCE; j++) {
                vertex.boneIndex[j] = -1;
            }

            vertices[i] = std::move(vertex);
        }

        for (uint32_t i = 0; i < mesh->mNumBones; i++) {
            parseBoneWeight(vertices, mesh->mBones[i]);
        }

        // process mesh's face
        Vector<uint32_t> indices;
        for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            if (face.mNumIndices != 3) {
                LOG_ERROR(logger, "vertex face is not triangluated! mesh triangle may be not rendered correctly.");
            }
            for (uint32_t j = 0; j < face.mNumIndices; j++) {
                indices.emplace_back(face.mIndices[j]);
            }
        }

        // normalize vertex bone weights to make all weights sum 1
        for (uint32_t i = 0; i < vertices.size(); i++) {
            real totalWeight = vertices[i].boneWeight.x + vertices[i].boneWeight.y + vertices[i].boneWeight.z + vertices[i].boneWeight.w;
            if (totalWeight > real(0)) {
                vertices[i].boneWeight /= totalWeight;
            }
        }

        VertexBuffer* vertexBuffer = NEW_T(VertexBuffer(vertexDesc, vertices.size()));
        vertexBuffer->copyBufferData(vertices.data());
        IndexBuffer* indexBuffer = NEW_T(IndexBuffer(sizeof(uint32_t), indices.size()));
        indexBuffer->copyBufferData(indices.data());
        return Platform::getRenderer().createMesh(vertexBuffer, indexBuffer);
    }

    void AssimpLoader::parseBoneWeight(Vector<Vertex>& vertices, aiBone* bone) {
        int32_t boneID = findOrCacheBone(bone->mName.C_Str(), bone);
        aiVertexWeight* weights = bone->mWeights;
        uint32_t numWeights = bone->mNumWeights;

        for (uint32_t j = 0; j < numWeights; j++) {
            uint32_t vertexID = weights[j].mVertexId;
            real weight = weights[j].mWeight;
            if (vertexID > vertices.size()) {
                LOG_ERROR(logger, "Failed to parse bone weight! vertexID(`{0}`) should not be greater than vertices size (`{1}`)", vertexID, vertices.size());
                return;
            }
            Vertex& vertex = vertices[vertexID];

            for (uint32_t k = 0; k < MAX_BONE_INFLUENCE; k++) {
                if (vertex.boneIndex[k] < 0) {
                    vertex.boneWeight[k] = weight;
                    vertex.boneIndex[k] = boneID;
                }
            }
        }
    }

    BoneInfo* AssimpLoader::findBone(const std::string& name) {
        if (mBoneMap.find(name) != mBoneMap.end()) {
            return &mBoneMap[name];
        }
        return nullptr;
    }

    int32_t AssimpLoader::findOrCacheBone(const std::string& name, aiBone* bone) {
        // already exists in bone map
        BoneInfo* foundBone = findBone(name);
        if (foundBone != nullptr) {
            return foundBone->id;
        }

        BoneInfo boneInfo{};
        boneInfo.id = mNumBoneCount++;
        boneInfo.name = name;
        boneInfo.offset = toMat4(bone->mOffsetMatrix);

        return mBoneMap.insert(std::make_pair(name, boneInfo)).first->second.id;
    }

    Texture* AssimpLoader::loadTexture(aiMaterial* material, aiTextureType textureType) {
        aiString name;
        if (material->GetTexture(textureType, 0, &name) != AI_SUCCESS) {
            LOG_ERROR(logger, "failed to load texture (type `{0}`)", textureType);
            return nullptr;
        }
        // replace windows specific path separators
        std::string texturePath = mDirectoryPath + StringUtils::replaceAll(name.C_Str(), "\\", PATH_SEPARATOR);
        return Platform::getRenderer().createTexture2D(texturePath, PixelFormat::RGBA32); // FIXME: format? automatic
    }

    void AssimpLoader::loadBone(Bone& targetBone, const aiNode* node) {
        std::string boneName = node->mName.C_Str();
        BoneInfo* boneInfo = findBone(boneName);

        // filtering mesh node (we only care about bone node)
        if (boneInfo == nullptr) {
            for (uint32_t i = 0; i < node->mNumChildren; i++) {
                loadBone(targetBone, node->mChildren[i]);
            }
        } else {
            targetBone.name = boneName;
            targetBone.boneTransformation = toMat4(node->mTransformation);

            for (uint32_t i = 0; i < node->mNumChildren; i++) {
                Bone childBone;
                loadBone(childBone, node->mChildren[i]);
                targetBone.children.emplace_back(childBone);
            }
        }
    }

    void AssimpLoader::loadAnimation(const aiScene* scene) {
        for (uint32_t i = 0; i < scene->mNumAnimations; i++) {
            aiAnimation* animation = scene->mAnimations[i];

            Animation* anim = NEW_T(Animation);
            anim->duration = static_cast<real>(animation->mDuration);
            anim->ticksPerSecond = static_cast<real>(animation->mTicksPerSecond);
            anim->clip = NEW_T(AnimationClip(animation->mName.C_Str()));

            for (uint32_t j = 0; j < animation->mNumChannels; j++) {
                aiNodeAnim* channel = animation->mChannels[j];
                TransformCurve transformCurve;
                transformCurve.mBoneName = channel->mNodeName.C_Str();

                // load position keyframe
                for (uint32_t k = 0; k < channel->mNumPositionKeys; k++) {
                    aiVector3D vec = channel->mPositionKeys[k].mValue;
                    real value[3] = {vec.x, vec.y, vec.z};
                    transformCurve.mTranslation.addKeyFrame(KeyFrame<3>{
                        static_cast<real>(channel->mPositionKeys[k].mTime), value
                    });
                }

                // load rotation keyframe
                for (uint32_t k = 0; k < channel->mNumRotationKeys; k++) {
                    aiQuaternion quat = channel->mRotationKeys[k].mValue;
                    real value[4] = {quat.w, quat.x, quat.y, quat.z};
                    transformCurve.mRotation.addKeyFrame(KeyFrame<4>{
                        static_cast<real>(channel->mRotationKeys[k].mTime), value
                    });
                }

                // load scale keyframe
                for (uint32_t k = 0; k < channel->mNumScalingKeys; k++) {
                    aiVector3D vec = channel->mScalingKeys[k].mValue;
                    real value[3] = {vec.x, vec.y, vec.z};
                    transformCurve.mScale.addKeyFrame(KeyFrame<3>{
                        static_cast<real>(channel->mScalingKeys[k].mTime), value
                    });
                }

                anim->clip->addCurve(transformCurve);
            }
            mAnimations.emplace_back(anim);
        }
    }

    Mat4<real> AssimpLoader::toMat4(const aiMatrix4x4& mat) {
        return Mat4<real>{
            mat.a1, mat.b1, mat.c1, mat.d1,
            mat.a2, mat.b2, mat.c2, mat.d2,
            mat.a3, mat.b3, mat.c3, mat.d3,
            mat.a4, mat.b4, mat.c4, mat.d4
        };
    }

    Vec3 AssimpLoader::toVec3(const aiVector3D& vec3) {
        return Vec3{vec3.x, vec3.y, vec3.z};
    }

    Vec2 AssimpLoader::toVec2(const aiVector3D& vec3) {
        return Vec2{vec3.x, vec3.y};
    }
}