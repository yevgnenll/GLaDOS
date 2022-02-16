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
#include "platform/OSTypes.h"
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
    const Array<aiTextureType, 10> AssimpLoader::SUPPORT_TEXTURE_TYPES = {
        aiTextureType_AMBIENT, // Blinn–Phong AO
        aiTextureType_DIFFUSE, // Blinn–Phong albedo
        aiTextureType_SPECULAR, // Blinn–Phong roughness
        aiTextureType_SHININESS, // Blinn–Phong shininess
        aiTextureType_NORMALS, // Blinn–Phong / PBR normal map
        aiTextureType_HEIGHT, // Height map (confusion, obj file's bump map will be mapped into Assimp Height map, byt map_Kn will be mapped into Assimp Normal map.
        aiTextureType_AMBIENT_OCCLUSION, // PBR AO
        aiTextureType_BASE_COLOR, // PBR albedo
        aiTextureType_DIFFUSE_ROUGHNESS, // PBR roughness
        aiTextureType_METALNESS // PBR metallic
    };

    AssimpLoader::AssimpLoader() {
        setDestructionPhase(1);
    }

    bool AssimpLoader::loadFromFile(const std::string& fileName, GameObject* parent) {
        std::string directoryPath = StringUtils::splitFileName(fileName).first;
        std::string filePath = std::string(RESOURCE_DIR) + fileName;

        Assimp::Importer importer;
        importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
        importer.SetPropertyBool(AI_CONFIG_IMPORT_REMOVE_EMPTY_BONES, false);
        unsigned int importFlags = aiProcessPreset_TargetRealtime_Quality;
#ifdef PLATFORM_MACOS
        // Metal API uses texture coordinates such as the origin(0, 0) is located at the top-left corner (Y down).
        importFlags |= aiProcess_FlipUVs;
#elif PLATFORM_WINDOW
        // there are already predefined DirectX12 import flags in Assimp.
        importFlags |= aiProcess_ConvertToLeftHanded;
#else
        // Vulkan API uses right-handed NDC space so default Assimp flag is enough
        // but Vulkan API uses texture coordinates as same as Metal API so flip this.
        importFlags |= aiProcess_FlipUVs;
#endif
        const aiScene* scene = importer.ReadFile(filePath, importFlags);

        if (scene == nullptr || ((scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0u) || scene->mRootNode == nullptr) {
            LOG_ERROR(logger, "Load from file error: {0}", importer.GetErrorString());
            return false;
        }

        if (!scene->HasMeshes()) {
            LOG_ERROR(logger, "Scene is without meshes.");
            return false;
        }

        aiNode* rootNode = scene->mRootNode;
        Mat4<real> rootToWorld = Mat4<real>::inverse(toMat4(rootNode->mTransformation));
        parent->transform()->decomposeSRT(rootToWorld);

        // first build all node in scene
        int32_t numBones = 0;
        UnorderedMap<std::string, SceneNode*> nodeMap;
        for (uint32_t i = 0; i < rootNode->mNumChildren; i++) {
            buildNodeMap(rootNode->mChildren[i], numBones, nodeMap);
        }

        // load bone hierarchy
        for (uint32_t i = 0; i < rootNode->mNumChildren; i++) {
            GameObject* boneNode = buildBoneHierarchy(rootNode->mChildren[i], parent, nodeMap);
            if (boneNode != nullptr) {
                parent->getChildren().emplace_back(boneNode);
            }
        }

        // load node mesh and material
        GameObject* rootBoneNode = nullptr;
        if (!parent->getChildren().empty()) {
            rootBoneNode = parent->getChildren().front();
        }
        Vector<Mesh*> meshes = loadNodeMeshAndMaterial(rootNode, scene, parent, rootBoneNode, nodeMap, directoryPath);

        // setting bind pose
        if (rootBoneNode != nullptr) {
            Vector<Mat4<real>> bindPoses;
            getBindPose(bindPoses, nodeMap);
            for (Mesh* mesh : meshes) {
                mesh->setBindPose(bindPoses);
            }
        }

        // load animations
        Vector<AnimationClip*> animationClips = loadAnimation(scene, rootBoneNode, nodeMap);
        if (!animationClips.empty()) {
            Animator* animator = parent->addComponent<Animator>();
            for (AnimationClip* clip : animationClips) {
                animator->addClip(clip, clip->getName());
            }
        }

        deallocValueInMap(nodeMap);

        return true;
    }

    Vector<Mesh*> AssimpLoader::loadNodeMeshAndMaterial(aiNode* node, const aiScene* scene,
                                                        GameObject* parent, GameObject* rootBone,
                                                        UnorderedMap<std::string, SceneNode*>& nodeMap, const std::string& textureRootPath) {
        Vector<Mesh*> meshes;

        // load mesh, material at the current node
        for (uint32_t i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

            Mesh* currentMesh = loadMesh(mesh, nodeMap);
            Material* currentMaterial = loadMaterial(material, rootBone, textureRootPath);

            if (currentMesh != nullptr && currentMaterial != nullptr) {
                createGameObject(mesh->mName.C_Str(), currentMesh, currentMaterial, parent, rootBone);
                meshes.emplace_back(currentMesh);
            }
        }

        // recursively load all the child node
        for (uint32_t j = 0; j < node->mNumChildren; j++) {
            Vector<Mesh*> childMeshes = loadNodeMeshAndMaterial(node->mChildren[j], scene, parent, rootBone, nodeMap, textureRootPath);
            std::copy(childMeshes.begin(), childMeshes.end(), std::back_inserter(meshes));
        }

        return meshes;
    }

    Mesh* AssimpLoader::loadMesh(aiMesh* mesh, UnorderedMap<std::string, SceneNode*>& nodeMap) {
        VertexFormatDescriptor vertexDesc = VertexFormatDescriptor().position().normal().tangent().biTangent().boneWeight().boneIndex().texCoord0();

        // process mesh's vertices
        Vector<Vertex> vertices(mesh->mNumVertices);
        for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;
            if (!mesh->HasPositions()) {
                LOG_ERROR(logger, "Has no vertex positions.");
                return nullptr;
            }
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

        // process mesh's face
        Vector<uint32_t> indices(mesh->mNumFaces * 3);
        for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
            aiFace& face = mesh->mFaces[i];
            if (face.mNumIndices != 3) {
                LOG_ERROR(logger, "vertex face is not triangluated! mesh triangle may be not rendered correctly.");
            }
            for (uint32_t j = 0; j < face.mNumIndices; j++) {
                indices[(i * 3) + j] = face.mIndices[j];
            }
        }

        // process mesh's bone weight & bone index
        for (uint32_t i = 0; i < mesh->mNumBones; i++) {
            aiBone* bone = mesh->mBones[i];
            SceneNode* node = findNode(bone->mName.C_Str(), nodeMap);
            if (node == nullptr || !node->isBone) {
                LOG_ERROR(logger, "Can't find `{0}` bone in scene nodes", bone->mName.C_Str());
                continue;
            }
            node->offset = toMat4(bone->mOffsetMatrix);

            for (uint32_t j = 0; j < bone->mNumWeights; j++) {
                uint32_t vertexID = bone->mWeights[j].mVertexId;
                real weight = bone->mWeights[j].mWeight;
                if (vertexID >= vertices.size()) {
                    LOG_ERROR(logger, "Failed to parse bone weight! vertexID(`{0}`) should not be greater than vertices size (`{1}`)", vertexID, vertices.size());
                    continue;
                }
                Vertex& vertex = vertices[vertexID];

                for (uint32_t k = 0; k < MAX_BONE_INFLUENCE; k++) {
                    if (vertex.boneIndex[k] < 0) {
                        vertex.boneWeight[k] = weight;
                        vertex.boneIndex[k] = node->id;
                        break;
                    }
                }
            }
        }

        VertexBuffer* vertexBuffer = NEW_T(VertexBuffer(vertexDesc, vertices.size()));
        vertexBuffer->copyBufferData(vertices.data());
        IndexBuffer* indexBuffer = NEW_T(IndexBuffer(sizeof(uint32_t), indices.size()));
        indexBuffer->copyBufferData(indices.data());
        return Platform::getRenderer().createMesh(vertexBuffer, indexBuffer);
    }

    Material* AssimpLoader::loadMaterial(aiMaterial* material, GameObject* rootBone, const std::string& textureRootPath) {
        ShaderProgram* shaderProgram;
        if (rootBone != nullptr) {
            shaderProgram = Platform::getRenderer().createShaderProgramFromFile("skinningVertex", "skinningFragment");
            if (shaderProgram == nullptr) {
                return nullptr;
            }
        } else {
            shaderProgram = Platform::getRenderer().createShaderProgramFromFile("normalVertex", "normalFragment");
            if (shaderProgram == nullptr) {
                return nullptr;
            }
        }

        Material* mat = NEW_T(Material);
        mat->setShaderProgram(shaderProgram);

        aiColor4D diffuse;
        if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse)) {
            mat->setAlbedo(toColor(diffuse));
        }

        aiColor4D specular;
        if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_SPECULAR, specular)) {
            mat->setSpecular(toColor(specular));
        }

        std::size_t lastTexture = 0;
        for (const auto textureType : SUPPORT_TEXTURE_TYPES) {
            if (material->GetTextureCount(textureType) > 0) {
                Texture* texture = loadTexture(material, textureType, textureRootPath);
                if (texture != nullptr) {
                    mat->setTextureFromIndex(texture, lastTexture);
                    mat->setTextureType(toTextureType(textureType), lastTexture);
                    lastTexture++;
                }
            }
        }

        return mat;
    }

    Texture* AssimpLoader::loadTexture(aiMaterial* material, aiTextureType textureType, const std::string& textureRootPath) {
        aiString name;
        if (material->GetTexture(textureType, 0, &name) != AI_SUCCESS) {
            LOG_ERROR(logger, "Failed to load texture (type `{0}`)", textureType);
            return nullptr;
        }
        // replace windows specific path separators
        std::string texturePath = textureRootPath + StringUtils::replaceAll(name.C_Str(), "\\", PATH_SEPARATOR);
        return Platform::getRenderer().createTexture2D(texturePath, PixelFormat::RGBA32);
    }

    Vector<AnimationClip*> AssimpLoader::loadAnimation(const aiScene* scene, GameObject* rootNode, UnorderedMap<std::string, SceneNode*>& nodeMap) {
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
            clip->setInitialTicksPerSecond(static_cast<real>(animation->mTicksPerSecond));

            for (uint32_t j = 0; j < animation->mNumChannels; j++) {
                aiNodeAnim* channel = animation->mChannels[j];
                SceneNode* sceneNode = findNode(channel->mNodeName.C_Str(), nodeMap);
                if (sceneNode == nullptr || !sceneNode->isBone) {
                    LOG_ERROR(logger, "Can't find bone in animation: `{0}`", channel->mNodeName.C_Str());
                    continue;
                }

                TransformCurve transformCurve;
                GameObject* targetBone = retrieveTargetBone(sceneNode->name, rootNode);
                if (targetBone == nullptr) {
                    LOG_ERROR(logger, "Can't find target bone `{0}`", sceneNode->name);
                    continue;
                }
                transformCurve.mTargetBone = targetBone;

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
                    real value[4] = {-quat.w, quat.x, quat.y, quat.z};
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

    void AssimpLoader::buildNodeMap(const aiNode* node, int32_t& boneCounter, UnorderedMap<std::string, SceneNode*>& nodeMap) {
        std::string nodeName = node->mName.C_Str();
        SceneNode* newNode = NEW_T(SceneNode);
        newNode->isBone = (node->mNumMeshes == 0);
        newNode->id = (newNode->isBone) ? boneCounter++ : -1;
        newNode->name = nodeName;
        nodeMap.insert(std::make_pair(newNode->name, newNode));

        // recursively load all the child node
        for (uint32_t i = 0; i < node->mNumChildren; i++) {
            buildNodeMap(node->mChildren[i], boneCounter, nodeMap);
        }
    }

    GameObject* AssimpLoader::buildBoneHierarchy(const aiNode* node, GameObject* parent, UnorderedMap<std::string, SceneNode*>& nodeMap) {
        SceneNode* sceneNode = findNode(node->mName.C_Str(), nodeMap);
        if (sceneNode == nullptr || !sceneNode->isBone) {
            return nullptr;
        }
        GameObject* boneNode = parent->scene()->createGameObject(sceneNode->name, parent);
        boneNode->transform()->decomposeSRT(toMat4(node->mTransformation));

        for (uint32_t i = 0; i < node->mNumChildren; i++) {
            GameObject* childRigGameObject = buildBoneHierarchy(node->mChildren[i], boneNode, nodeMap);
            if (childRigGameObject != nullptr) {
                boneNode->getChildren().emplace_back(childRigGameObject);
            }
        }

        return boneNode;
    }

    void AssimpLoader::getBindPose(Vector<Mat4<real>>& bindPose, UnorderedMap<std::string, SceneNode*>& nodeMap) {
        Vector<SceneNode*> sceneNodes;
        for (const auto& pair : nodeMap) {
            if (pair.second->isBone) {
                sceneNodes.emplace_back(pair.second);
            }
        }
        // sort ascending order
        std::sort(sceneNodes.begin(), sceneNodes.end(), [](SceneNode* node1, SceneNode* node2) { return node1->id < node2->id; });
        std::transform(sceneNodes.begin(), sceneNodes.end(), std::back_inserter(bindPose), [](SceneNode* node) { return node->offset; });
    }

    SceneNode* AssimpLoader::findNode(const std::string& name, UnorderedMap<std::string, SceneNode*>& nodeMap) {
        if (nodeMap.find(name) != nodeMap.end()) {
            return nodeMap[name];
        }
        return nullptr;
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

    void AssimpLoader::createGameObject(const std::string& name, Mesh* mesh, Material* material, GameObject* parent, GameObject* rootBone) {
        GameObject* node = parent->scene()->createGameObject(name, parent);
        if (rootBone != nullptr) {
            node->addComponent<SkinnedMeshRenderer>(mesh, material, rootBone);
        } else {
            node->addComponent<MeshRenderer>(mesh, material);
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

    Color AssimpLoader::toColor(const aiColor4D& color) {
        return Color{color.r, color.g, color.b, color.a};
    }

    TextureType AssimpLoader::toTextureType(aiTextureType textureType) {
        TextureType result;
        switch (textureType) {
            case aiTextureType_AMBIENT:
                result = TextureType::Ambient;
                break;
            case aiTextureType_DIFFUSE:
                result = TextureType::Diffuse;
                break;
            case aiTextureType_SPECULAR:
                result = TextureType::Specular;
                break;
            case aiTextureType_SHININESS:
                result = TextureType::Shininess;
                break;
            case aiTextureType_NORMALS:
                result = TextureType::NormalMap;
                break;
            case aiTextureType_HEIGHT:
                result = TextureType::HeightMap;
                break;
            case aiTextureType_AMBIENT_OCCLUSION:
                result = TextureType::AmbientOcclusion;
                break;
            case aiTextureType_BASE_COLOR:
                result = TextureType::Albedo;
                break;
            case aiTextureType_DIFFUSE_ROUGHNESS:
                result = TextureType::Roughness;
                break;
            case aiTextureType_METALNESS:
                result = TextureType::Metallic;
                break;
            default:
                result = TextureType::Undefined;
                break;
        }
        return result;
    }
}