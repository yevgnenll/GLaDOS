#include "AssimpLoader.h"
#include "math/Vec3.h"
#include "utils/LoggerRegistry.h"
#include "utils/Utility.h"
#include "utils/StringUtils.h"
#include "platform/Platform.h"
#include "platform/render/IndexBuffer.h"
#include "platform/render/Material.h"
#include "platform/render/Mesh.h"
#include "platform/render/Renderer.h"
#include "platform/render/VertexBuffer.h"
#include "platform/render/Texture2D.h"
#include "core/animation/TransformCurve.h"
#include "core/animation/AnimationClip.h"
#include "core/component/renderer/SkinnedMeshRenderer.h"
#include "core/component/renderer/MeshRenderer.h"
#include "core/component/Transform.h"
#include "core/component/Animator.h"
#include "core/GameObject.hpp"
#include "core/Scene.h"
#include "RootDir.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace GLaDOS {
    Logger* AssimpLoader::logger = LoggerRegistry::getInstance().makeAndGetLogger("AssimpLoader");
    const uint32_t AssimpLoader::MAX_BONE_INFLUENCE = 4;

    bool AssimpLoader::loadFromFile(const std::string& fileName, Scene* scene, GameObject* parent) {
        mDirectoryPath = StringUtils::splitFileName(fileName).first;
        std::string filePath = std::string(RESOURCE_DIR) + fileName;

        Assimp::Importer importer;
        const aiScene* aiscene = importer.ReadFile(filePath, aiProcessPreset_TargetRealtime_Quality);
        if (aiscene == nullptr || ((aiscene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0u) || aiscene->mRootNode == nullptr) {
            LOG_ERROR(logger, "Load from file error: {0}", importer.GetErrorString());
            return false;
        }

        aiNode* rootNode = aiscene->mRootNode;
        // TODO: rootToWorld apply in root bone node?
        Mat4<real> rootToWorld = Mat4<real>::inverse(toMat4(rootNode->mTransformation));

        // first build all node in scene
        int32_t numBones = 0;
        for (uint32_t i = 0; i < rootNode->mNumChildren; i++) {
            buildNodeTable(rootNode->mChildren[i], numBones);
        }

        // load bone hierarchy
        for (uint32_t i = 0; i < rootNode->mNumChildren; i++) {
            GameObject* boneNode = buildBoneHierarchy(rootNode->mChildren[i], scene, parent);
            if (boneNode != nullptr) {
                parent->getChildren().emplace_back(boneNode);
            }
        }

        // load node mesh and material
        GameObject* rootBoneNode = nullptr;
        if (!parent->getChildren().empty()) {
            rootBoneNode = parent->getChildren().front();
        }
        Vector<Mesh*> meshes = loadNodeMeshAndMaterial(rootNode, aiscene, scene, parent, rootBoneNode);
        Vector<Mat4<real>> bindPoses;
        getBindPose(bindPoses);
        for (Mesh* mesh : meshes) {
            mesh->setBindPose(bindPoses);
        }

        // load animations
        if (rootBoneNode != nullptr) {
            Vector<AnimationClip*> animationClips = loadAnimation(aiscene, rootBoneNode);
            if (!animationClips.empty()) {
                Animator* animator = parent->addComponent<Animator>();
                for (AnimationClip* clip : animationClips) {
                    animator->addClip(clip, clip->getName());
                }
            }
        }

        return true;
    }

    Vector<Texture*> AssimpLoader::getTexture() const {
        return mTextures;
    }

    Vector<Mesh*> AssimpLoader::loadNodeMeshAndMaterial(aiNode* node, const aiScene* aiscene, Scene* scene, GameObject* parent, GameObject* rootBone) {
        static const Array<aiTextureType, 4> textureTypes = { aiTextureType_DIFFUSE, aiTextureType_SPECULAR, aiTextureType_AMBIENT, aiTextureType_NORMALS };
        Vector<Mesh*> meshes;

        // load mesh at the current node
        for (uint32_t i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = aiscene->mMeshes[node->mMeshes[i]];
            Mesh* currentMesh = loadMesh(mesh);
            if (currentMesh != nullptr) {
                makeGameObject(mesh->mName.C_Str(), currentMesh, scene, parent, rootBone);
                meshes.emplace_back(currentMesh);
            }

            aiMaterial* material = aiscene->mMaterials[mesh->mMaterialIndex];
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
            Vector<Mesh*> childMeshes = loadNodeMeshAndMaterial(node->mChildren[j], aiscene, scene, parent, rootBone);
            std::copy(childMeshes.begin(), childMeshes.end(), std::back_inserter(meshes));
        }

        return meshes;
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
        if (node->offset.isIdentity()) {
            node->offset = toMat4(bone->mOffsetMatrix);
        }

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

    void AssimpLoader::buildNodeTable(const aiNode* node, int32_t& boneCounter) {
        std::string nodeName = node->mName.C_Str();
        SceneNode newNode;
        newNode.id = (node->mNumMeshes != 0) ? -1 : boneCounter++;
        newNode.name = nodeName;
        newNode.isBone = (node->mNumMeshes == 0);
        addNode(newNode);

        // recursively load all the child node
        for (uint32_t i = 0; i < node->mNumChildren; i++) {
            buildNodeTable(node->mChildren[i], boneCounter);
        }
    }

    GameObject* AssimpLoader::buildBoneHierarchy(const aiNode* node, Scene* scene, GameObject* parent) {
        SceneNode* sceneNode = findNode(node->mName.C_Str());
        if (sceneNode == nullptr || !sceneNode->isBone) { // FIXME: bug may appear here
            return nullptr;
        }
        GameObject* boneNode = scene->createGameObject(sceneNode->name, parent);
        boneNode->transform()->fromMat4(toMat4(node->mTransformation));

        for (uint32_t i = 0; i < node->mNumChildren; i++) {
            GameObject* childRigGameObject = buildBoneHierarchy(node->mChildren[i], scene, boneNode);
            if (childRigGameObject != nullptr) {
                boneNode->getChildren().emplace_back(childRigGameObject);
            }
        }

        return boneNode;
    }

    Vector<AnimationClip*> AssimpLoader::loadAnimation(const aiScene* scene, GameObject* rootNode) {
        Vector<AnimationClip*> clips;
        for (uint32_t i = 0; i < scene->mNumAnimations; i++) {
            aiAnimation* animation = scene->mAnimations[i];
            std::string animationName = animation->mName.C_Str();
            if (animationName.empty()) {
                animationName = "animation[" + StringUtils::normalize(clips.size()) + "]";
            }
            AnimationClip* clip = NEW_T(AnimationClip(animationName));
            clip->setStartTime(0);
            clip->setEndTime(static_cast<real>(animation->mDuration));

            for (uint32_t j = 0; j < animation->mNumChannels; j++) {
                aiNodeAnim* channel = animation->mChannels[j];
                SceneNode* sceneNode = findNode(channel->mNodeName.C_Str());
                if (sceneNode == nullptr || !sceneNode->isBone) {
                    LOG_ERROR(logger, "Can't find bone in animation: `{0}`", channel->mNodeName.C_Str());
                    return clips;
                }

                TransformCurve transformCurve;
                transformCurve.mTargetBone = retrieveTargetBone(sceneNode->name, rootNode);
                // TODO: customize interpolation type in aiNodeAnim

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

                clip->addCurve(transformCurve);
            }
            clips.emplace_back(clip);
        }

        return clips;
    }

    void AssimpLoader::getBindPose(Vector<Mat4<real>>& bindPose) {
        Vector<SceneNode> sceneNodes;
        // TODO: remove copy
        for (const auto& pair : mNodeTable) {
            if (pair.second.isBone) {
                sceneNodes.emplace_back(pair.second);
            }
        }
        std::sort(sceneNodes.begin(), sceneNodes.end(), [](const SceneNode& node1, const SceneNode& node2) { return node1.id < node2.id; });
        std::transform(sceneNodes.begin(), sceneNodes.end(), std::back_inserter(bindPose), [](const SceneNode& node) { return node.offset; });
    }

    SceneNode* AssimpLoader::findNode(const std::string& name) {
        if (mNodeTable.find(name) != mNodeTable.end()) {
            return &mNodeTable[name];
        }
        return nullptr;
    }

    int32_t AssimpLoader::addNode(const SceneNode& node) {
        return mNodeTable.insert(std::make_pair(node.name, node)).first->second.id;
    }

    GameObject* AssimpLoader::retrieveTargetBone(const std::string& name, GameObject* rootNode) {
        if (rootNode == nullptr) {
            return nullptr;
        }
        if (rootNode->getName() == name) {
            return rootNode;
        }
        for (uint32_t i = 0; i < rootNode->getChildren().size(); i++) {
            GameObject* node = retrieveTargetBone(name, rootNode->getChildren()[i]);
            if (node != nullptr) {
                return node;
            }
        }
        return nullptr;
    }

    void AssimpLoader::makeGameObject(const std::string& name, Mesh* mesh, Scene* scene, GameObject* parent, GameObject* rootBone) {
        GameObject* node = scene->createGameObject(name, parent);
        ShaderProgram* shaderProgram = Platform::getRenderer().createShaderProgramFromFile("skinningVertex", "skinningFragment");
        if (shaderProgram == nullptr) {
            return;
        }
        Material* material = NEW_T(Material);
        material->setShaderProgram(shaderProgram);

        if (rootBone != nullptr) {
            node->addComponent<SkinnedMeshRenderer>(mesh, material, rootBone);
            return;
        }
        node->addComponent<MeshRenderer>(mesh, material);
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