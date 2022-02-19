#ifndef GLADOS_TEXTURE_H
#define GLADOS_TEXTURE_H

#include "resource/Resource.h"
#include "utils/Utility.h"

namespace GLaDOS {
    class Logger;
    class Color;
    class SamplerState;
    struct SamplerDescription;
    class Blob;
    class Texture : public Resource {
      public:
        Texture(const std::string& name, PixelFormat format);
        ~Texture() override;

        SamplerState* samplerState();
        void setSamplerState(const SamplerDescription& desc);
        void setColorKey(const Color& colorKey);
        Color colorKey() const;
        uint32_t getWidth() const;
        uint32_t getHeight() const;
        void setWidth(uint32_t width);
        void setHeight(uint32_t height);
        uint32_t getChannels() const;
        bool isUseMipmap() const;
        uint32_t mipmapCount() const;
        PixelFormat getPixelFormat() const;
        TextureDimension getDimension() const;
        void overrideUsage(TextureUsage textureUsage);

        virtual bool loadTextureFromFile() { return false; }
        virtual bool loadTextureFromBuffer(Blob& buffer) { return false; }
        virtual bool loadTextureFromFile(const Array<std::string, 6>& names) { return false; }
        virtual bool loadTextureFromBuffer(const Vector<std::reference_wrapper<Blob>>& buffer) { return false; }

      protected:
        static uint32_t mapChannelNumberFrom(PixelFormat format);

        uint32_t mWidth{0};
        uint32_t mHeight{0};
        uint32_t mChannels{0};
        bool mUseMipmap{true};
        uint32_t mMipmapCount{1};
        PixelFormat mFormat{PixelFormat::Unknown};
        TextureUsage mUsage{TextureUsage::Unknown};
        TextureDimension mDimension{TextureDimension::Unknown};
        SamplerState* mSamplerState{nullptr};
        Color* mColorKey{nullptr};

      private:
        static Logger* logger;
    };
}  // namespace GLaDOS

#endif  //GLADOS_TEXTURE_H
