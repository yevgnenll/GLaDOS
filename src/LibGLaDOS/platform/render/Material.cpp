#include "Material.h"

#include "ShaderProgram.h"
#include "Texture.h"
#include "platform/Platform.h"
#include "platform/render/Renderer.h"
#include "platform/render/RenderState.h"

namespace GLaDOS {
    Material::Material(ShaderProgram* shaderProgram) : mShaderProgram{shaderProgram} {

    }

    Material::~Material() {
        DELETE_T(mShaderProgram, ShaderProgram);
    }

    Material::Material(const Material& other) : mTextures{other.mTextures} {
        mShaderProgram = Platform::getRenderer().createShaderProgram(other.mShaderProgram->getVertexShader(), other.mShaderProgram->getFragmentShader());
        RasterizerDescription rasterizerDescription = other.mShaderProgram->rasterizerState()->mRasterizerDescription;
        DepthStencilDescription depthStencilDescription = other.mShaderProgram->depthStencilState()->mDepthStencilDescription;
        mShaderProgram->setRasterizerState(rasterizerDescription);
        mShaderProgram->setDepthStencilState(depthStencilDescription);
    }

    Material& Material::operator=(const Material& other) {
        mTextures = other.mTextures;
        mShaderProgram = Platform::getRenderer().createShaderProgram(mShaderProgram->getVertexShader(), mShaderProgram->getFragmentShader());
        RasterizerDescription rasterizerDescription = other.mShaderProgram->rasterizerState()->mRasterizerDescription;
        DepthStencilDescription depthStencilDescription = other.mShaderProgram->depthStencilState()->mDepthStencilDescription;
        mShaderProgram->setRasterizerState(rasterizerDescription);
        mShaderProgram->setDepthStencilState(depthStencilDescription);
        return *this;
    }

    ShaderProgram* Material::getShaderProgram() const {
        return mShaderProgram;
    }

    void Material::setShaderProgram(ShaderProgram* shaderProgram) {
        mShaderProgram = shaderProgram;
    }

    Texture* Material::getTexture0() {
        return mTextures[0];
    }

    void Material::setTexture0(Texture* texture) {
        mTextures[0] = texture;
    }

    Texture* Material::getTexture1() {
        return mTextures[1];
    }

    void Material::setTexture1(Texture* texture) {
        mTextures[1] = texture;
    }

    Texture* Material::getTexture2() {
        return mTextures[2];
    }

    void Material::setTexture2(Texture* texture) {
        mTextures[2] = texture;
    }

    Texture* Material::getTexture3() {
        return mTextures[3];
    }

    void Material::setTexture3(Texture* texture) {
        mTextures[3] = texture;
    }

    Texture* Material::getTexture4() {
        return mTextures[4];
    }

    void Material::setTexture4(Texture* texture) {
        mTextures[4] = texture;
    }

    Texture* Material::getTexture5() {
        return mTextures[5];
    }

    void Material::setTexture5(Texture* texture) {
        mTextures[5] = texture;
    }

    Texture* Material::getTexture6() {
        return mTextures[6];
    }

    void Material::setTexture6(Texture* texture) {
        mTextures[6] = texture;
    }

    Texture* Material::getTexture7() {
        return mTextures[7];
    }

    void Material::setTexture7(Texture* texture) {
        mTextures[7] = texture;
    }

    Texture* Material::getTextureFromIndex(std::size_t index) {
        if (index < 0 || index >= MAX_TEXTURE_COUNT) {
            return nullptr;
        }
        return mTextures[index];
    }

    void Material::setTextureFromIndex(Texture* texture, std::size_t index) {
        if (index >= 0 || index < MAX_TEXTURE_COUNT) {
            mTextures[index] = texture;
        }
    }

    TextureType Material::getTextureType(std::size_t index) {
        if (index < 0 || index >= MAX_TEXTURE_COUNT) {
            return TextureType::Undefined;
        }
        return mTextureTypes[index];
    }

    void Material::setTextureType(TextureType textureType, std::size_t index) {
        if (index >= 0 || index < MAX_TEXTURE_COUNT) {
            mTextureTypes[index] = textureType;
        }
    }

    Color Material::getBaseColor() const {
        return mBaseColor;
    }

    void Material::setBaseColor(const Color& color) {
        mBaseColor = color;
    }
}  // namespace GLaDOS