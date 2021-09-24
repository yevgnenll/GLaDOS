#ifndef GLADOS_TEXTURE3D_H
#define GLADOS_TEXTURE3D_H

#include "Texture.h"

namespace GLaDOS {
    class Texture3D : public Texture {
      public:
        Texture3D(const std::string& name, PixelFormat format);
        ~Texture3D() override;

        bool loadTextureFromFile() override;
        bool loadTextureFromBuffer(StreamBuffer& buffer) override;

      private:
        uint32_t mDepth{0};
    };
}  // namespace GLaDOS

#endif  //GLADOS_TEXTURE3D_H
