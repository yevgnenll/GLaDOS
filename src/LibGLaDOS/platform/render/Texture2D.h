#ifndef GLADOS_TEXTURE2D_H
#define GLADOS_TEXTURE2D_H

#include "Texture.h"

namespace GLaDOS {
    class Logger;
    class Texture2D : public Texture {
      public:
        Texture2D(const std::string& name, PixelFormat format);
        ~Texture2D() override;

        bool loadTextureFromFile() override;
        bool loadTextureFromBuffer(StreamBuffer& buffer) override;
        virtual void generateTexture(uint32_t x, uint32_t y, uint8_t* data) = 0;
        virtual void replaceRegion(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t level, uint8_t* data) = 0;

      protected:
        static uint32_t calculateMipmapsCount(uint32_t w, uint32_t h);
        bool generateMipmapsTexture(uint32_t x, uint32_t y, uint8_t* data);
        bool checkMipmapsUsable() const;

      private:
        static Logger* logger;
    };
}  // namespace GLaDOS

#endif  //GLADOS_TEXTURE2D_H