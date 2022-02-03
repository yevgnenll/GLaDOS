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

        aiNode* rootNode = scene->mRootNode;
        mRootTransform = Mat4<real>::inverse(toMat4(rootNode->mTransformation));

        // first build all node in scene
        for (uint32_t i = 0; i < rootNode->mNumChildren; i++) {
            buildNodeTable(rootNode->mChildren[i]);
        }

        // load node mesh and material
        loadNodeData(rootNode, scene);

        // load bone hierarchy
        for (uint32_t i = 0; i < rootNode->mNumChildren; i++) {
            buildBoneHierarchy(rootNode->mChildren[i], mRootBone);
        }

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

    int32_t AssimpLoader::getNodeCount() const {
        return mNumNodes;
    }

    Mat4<real> AssimpLoader::getRootTransform() const {
        return mRootTransform;
    }

    void AssimpLoader::loadNodeData(aiNode* node, const aiScene* scene) {
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
            loadNodeData(node->mChildren[j], scene);
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
            loadBoneWeight(vertices, mesh->mBones[i]);
        }

        // process mesh's face
        Vector<uint32_t> indices(mesh->mNumFaces * 3);
        for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            if (face.mNumIndices != 3) {
                LOG_ERROR(logger, "vertex face is not triangluated! mesh triangle may be not rendered correctly.");
            }
            for (uint32_t j = 0; j < face.mNumIndices; j++) {
                indices[(i * 3) + j] = face.mIndices[j];
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

    void AssimpLoader::loadBoneWeight(Vector<Vertex>& vertices, aiBone* bone) {
        SceneNode* node = findNode(bone->mName.C_Str());
        if (node == nullptr || !node->isBone) {
            LOG_ERROR(logger, "Can't find `{0}` node in node table", bone->mName.C_Str());
            return;
        }
        aiVertexWeight* weights = bone->mWeights;
        uint32_t numWeights = bone->mNumWeights;

        for (uint32_t i = 0; i < numWeights; i++) {
            uint32_t vertexID = weights[i].mVertexId;
            real weight = weights[i].mWeight;
            if (vertexID > vertices.size()) {
                LOG_ERROR(logger, "Failed to parse bone weight! vertexID(`{0}`) should not be greater than vertices size (`{1}`)", vertexID, vertices.size());
                return;
            }
            Vertex& vertex = vertices[vertexID];

            for (uint32_t j = 0; j < MAX_BONE_INFLUENCE; j++) {
                if (vertex.boneIndex[j] < 0) {
                    vertex.boneWeight[j] = weight;
                    vertex.boneIndex[j] = node->id;
                }
            }
        }
    }

    SceneNode* AssimpLoader::findNode(const std::string& name) {
        if (mNodeTable.find(name) != mNodeTable.end()) {
            return &mNodeTable[name];
        }
        return nullptr;
    }

    Texture* AssimpLoader::loadTexture(aiMaterial* material, aiTextureType textureType) {
        aiString name;
        if (material->GetTexture(textureType, 0, &name) != AI_SUCCESS) {
            LOG_ERROR(logger, "Failed to load texture (type `{0}`)", textureType);
            return nullptr;
        }
        // replace windows specific path separators
        std::string texturePath = mDirectoryPath + StringUtils::replaceAll(name.C_Str(), "\\", PATH_SEPARATOR);
        return Platform::getRenderer().createTexture2D(texturePath, PixelFormat::RGBA32); // FIXME: format? automatic
    }

    void AssimpLoader::buildNodeTable(const aiNode* node) {
        std::string nodeName = node->mName.C_Str();
        SceneNode* sceneNode = findNode(nodeName);

        // there is no sceneNode in node table yet.
        if (sceneNode == nullptr) {
            SceneNode newNode;
            newNode.id = mNumNodes++;
            newNode.name = nodeName;
            newNode.isBone = (node->mNumMeshes == 0);
            addNode(newNode);
        }

        // recursively load all the child node
        for (uint32_t i = 0; i < node->mNumChildren; i++) {
            buildNodeTable(node->mChildren[i]);
        }
    }

    int32_t AssimpLoader::addNode(const SceneNode& node) {
        return mNodeTable.insert(std::make_pair(node.name, node)).first->second.id;
    }

    void AssimpLoader::buildBoneHierarchy(const aiNode* node, Bone& targetBone) {
        SceneNode* sceneNode = findNode(node->mName.C_Str());
        if (sceneNode == nullptr || !sceneNode->isBone) { // FIXME: bug may appear here
            return;
        }
        targetBone.id = sceneNode->id;
        targetBone.name = sceneNode->name;
        targetBone.boneTransformation = toMat4(node->mTransformation);

        for (uint32_t i = 0; i < node->mNumChildren; i++) {
            Bone childBone;
            buildBoneHierarchy(node->mChildren[i], childBone);
            targetBone.children.emplace_back(childBone);
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
                SceneNode* sceneNode = findNode(channel->mNodeName.C_Str());
                if (sceneNode == nullptr || !sceneNode->isBone) {
                    LOG_ERROR(logger, "Can't find bone in animation");
                    return;
                }
                transformCurve.mBoneID = sceneNode->id;

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