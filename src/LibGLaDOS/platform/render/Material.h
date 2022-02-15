#ifndef GLADOS_MATERIAL_H
#define GLADOS_MATERIAL_H

#include "utils/Utility.h"
#include "math/Color.h"
#include "utils/Enumeration.h"

namespace GLaDOS {
    class Texture;
    class ShaderProgram;
    class Material {
      public:
        Material() = default;
        explicit Material(ShaderProgram* shaderProgram);
        ~Material();

        Material(const Material& other);
        Material& operator=(const Material& other);

        ShaderProgram* getShaderProgram() const;
        void setShaderProgram(ShaderProgram* shaderProgram);
        Texture* getTexture0();
        void setTexture0(Texture* texture);
        Texture* getTexture1();
        void setTexture1(Texture* texture);
        Texture* getTexture2();
        void setTexture2(Texture* texture);
        Texture* getTexture3();
        void setTexture3(Texture* texture);
        Texture* getTexture4();
        void setTexture4(Texture* texture);
        Texture* getTexture5();
        void setTexture5(Texture* texture);
        Texture* getTexture6();
        void setTexture6(Texture* texture);
        Texture* getTexture7();
        void setTexture7(Texture* texture);

        Texture* getTextureFromIndex(std::size_t index);
        void setTextureFromIndex(Texture* texture, std::size_t index);
        TextureType getTextureType(std::size_t index);
        void setTextureType(TextureType textureType, std::size_t index);
        Color getBaseColor() const;
        void setBaseColor(const Color& color);

      private:
        static constexpr const std::size_t MAX_TEXTURE_COUNT = 8;

        ShaderProgram* mShaderProgram{nullptr};
        Array<Texture*, MAX_TEXTURE_COUNT> mTextures{};
        Array<TextureType, MAX_TEXTURE_COUNT> mTextureTypes{TextureType::Undefined};
        Color mBaseColor;
    };
}  // namespace GLaDOS

#endif
