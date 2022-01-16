#ifndef GLADOS_MATERIAL_H
#define GLADOS_MATERIAL_H

#include "utils/Utility.h"

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

      private:
        ShaderProgram* mShaderProgram{nullptr};
        Array<Texture*, 8> mTextures{};
    };
}  // namespace GLaDOS

#endif
