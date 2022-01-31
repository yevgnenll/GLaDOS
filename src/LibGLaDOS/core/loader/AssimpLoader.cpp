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
#include "RootDir.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace GLaDOS {
    Logger* AssimpLoader::logger = LoggerRegistry::getInstance().makeAndGetLogger("AssimpLoader");

    bool AssimpLoader::loadFromFile(const std::string& filePath) {
        Assimp::Importer importer;
        std::string fileDirectory = std::string(RESOURCE_DIR) + filePath;
        const aiScene* scene = importer.ReadFile(fileDirectory, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
        if (scene == nullptr || ((scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0u) || scene->mRootNode == nullptr) {
            LOG_ERROR(logger, "Load from file error: {0}", importer.GetErrorString());
            return false;
        }

        mDirectoryPath = StringUtils::splitFileName(filePath).first;
        // load animations
        for (uint32_t i = 0; i < scene->mNumAnimations; i++) {
            mAnimationClips.emplace_back(loadAnimation(scene->mAnimations[i]));
        }

        return loadNode(scene->mRootNode, scene);
    }

    bool AssimpLoader::loadNode(aiNode* node, const aiScene* scene) {
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
            if (!loadNode(node->mChildren[j], scene)) {
                LOG_ERROR(logger, "failed to load node.");
                return false;
            }
        }

        return true;
    }

    Mesh* AssimpLoader::loadMesh(aiMesh* mesh) {
        Vector<Vertex> vertices(mesh->mNumVertices);
        VertexFormatDescriptor vertexDesc = generateVertexFormatDesc(mesh);

        // process mesh's vertices
        for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;
            vertex.position = Vec3{mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};

            if (mesh->HasNormals()) {
                vertex.normal = Vec3{mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
            }

            if (mesh->HasTangentsAndBitangents()) {
                vertex.tangent = Vec3{mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z};
                vertex.biTangent = Vec3{mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z};
            }

            if (mesh->HasTextureCoords(0)) {
                vertex.texcoord = Vec2{mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
            }
            vertex.boneIndex = 0;

            vertices[i] = std::move(vertex);
        }

        for (uint32_t i = 0; i < mesh->mNumBones; i++) {
            parseBoneWeight(vertices, mesh->mBones[i]);
        }

        Vector<uint32_t> indices;
        // process mesh's face
        for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            if (face.mNumIndices != 3) {
                LOG_ERROR(logger, "vertex face is not triangluated! mesh triangle may be not rendered correctly.");
            }
            for (uint32_t j = 0; j < face.mNumIndices; j++) {
                indices.emplace_back(face.mIndices[j]);
            }
        }

        VertexBuffer* vertexBuffer = NEW_T(VertexBuffer(vertexDesc, vertices.size()));
        vertexBuffer->copyBufferData(vertices.data());
        IndexBuffer* indexBuffer = NEW_T(IndexBuffer(sizeof(uint32_t), indices.size()));
        indexBuffer->copyBufferData(indices.data());
        return Platform::getRenderer().createMesh(vertexBuffer, indexBuffer);
    }

    void AssimpLoader::parseBoneWeight(Vector<Vertex>& vertices, aiBone* bone) {
        uint32_t boneID = findOrInsertJoint(bone->mName.C_Str(), bone);
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

            // packing boneIndex
            for (uint32_t k = 0; k < MAX_BONE_INFLUENCE; k++) {
                uint8_t boneIndex = static_cast<uint8_t>((vertex.boneIndex >> (8 * k)) & 0xFF);
                if (boneIndex <= 0) {
                    vertex.boneWeight[k] = weight;
                    vertex.boneIndex |= boneID << (8 * k);
                }
            }
        }
    }

    uint32_t AssimpLoader::findOrInsertJoint(const std::string& name, aiBone* bone) {
        // already exists in joint map
        if (mBoneMap.find(name) != mBoneMap.end()) {
            return mBoneMap[name].id;
        }

        BoneInfo boneInfo{};
        boneInfo.id = static_cast<uint32_t>(mBoneMap.size() + 1);
        boneInfo.name = name;
        boneInfo.offset = toMat4(bone->mOffsetMatrix);

        return mBoneMap.insert(std::make_pair(name, std::move(boneInfo))).first->second.id;
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

    AnimationClip* AssimpLoader::loadAnimation(aiAnimation* animation) {

        return nullptr;
    }

    Mat4<real> AssimpLoader::toMat4(const aiMatrix4x4& mat) {
        return Mat4<real>{
            mat.a1, mat.a2, mat.a3, mat.a4,
            mat.b1, mat.b2, mat.b3, mat.b4,
            mat.c1, mat.c2, mat.c3, mat.c4,
            mat.d1, mat.d2, mat.d3, mat.d4
        };
    }

    VertexFormatDescriptor AssimpLoader::generateVertexFormatDesc(aiMesh* mesh) {
        VertexFormatDescriptor vertexDesc;
        vertexDesc.position();

        if (mesh->HasNormals()) {
            vertexDesc.normal();
        }
        if (mesh->HasTangentsAndBitangents()) {
            vertexDesc.tangent();
            vertexDesc.biTangent();
        }

        for (uint32_t i = 0; i < 8; i++) {
            if (mesh->HasTextureCoords(i)) {
                vertexDesc.texCoord(i);
            }
        }

        if (mesh->HasBones()) {
            vertexDesc.boneWeight();
            vertexDesc.boneIndex();
        }

        return vertexDesc;
    }
}