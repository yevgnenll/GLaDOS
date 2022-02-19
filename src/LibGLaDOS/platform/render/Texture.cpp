#include "Texture.h"

#include "RenderState.h"
#include "Renderer.h"
#include "RootDir.h"
#include "math/Color.h"
#include "platform/Platform.h"

namespace GLaDOS {
    Logger* Texture::logger = LoggerRegistry::getInstance().makeAndGetLogger("Texture");
    Texture::Texture(const std::string& name, PixelFormat format) : Resource{ResourceType::Texture}, mFormat{format} {
        setResourceDir(RESOURCE_DIR);
        setName(name);
    }

    Texture::~Texture() {
        DELETE_T(mSamplerState, SamplerState);
        DELETE_T(mColorKey, Color);
    }

    SamplerState* Texture::samplerState() {
        if (mSamplerState == nullptr) {
            SamplerDescription defaultDesc{};
            setSamplerState(defaultDesc);
        }
        return mSamplerState;
    }

    void Texture::setSamplerState(const SamplerDescription& desc) {
        if (mSamplerState != nullptr) {
            DELETE_T(mSamplerState, SamplerState);
        }
        mSamplerState = Platform::getRenderer().createSamplerState(desc);
    }

    void Texture::setColorKey(const Color& colorKey) {
        if (mColorKey != nullptr) {
            DELETE_T(mColorKey, Color);
        }

        mColorKey = NEW_T(Color{colorKey});
    }

    Color Texture::colorKey() const {
        return *mColorKey;
    }

    uint32_t Texture::getWidth() const {
        return mWidth;
    }

    uint32_t Texture::getHeight() const {
        return mHeight;
    }

    void Texture::setWidth(uint32_t width) {
        mWidth = width;
    }

    void Texture::setHeight(uint32_t height) {
        mHeight = height;
    }

    uint32_t Texture::getChannels() const {
        return mChannels;
    }

    bool Texture::isUseMipmap() const {
        return mUseMipmap;
    }

    uint32_t Texture::mipmapCount() const {
        return mMipmapCount;
    }

    PixelFormat Texture::getPixelFormat() const {
        return mFormat;
    }

    TextureDimension Texture::getDimension() const {
        return mDimension;
    }

    void Texture::overrideUsage(TextureUsage textureUsage) {
        mUsage = textureUsage;
    }

    uint32_t Texture::mapChannelNumberFrom(PixelFormat format) {
        switch (format) {
            case PixelFormat::Red8:
                return 1;
            case PixelFormat::RG16:
                return 1;
            case PixelFormat::RGB24:  // metal doesn't support rgb format
            case PixelFormat::RGBA32:
                return 4;
            case PixelFormat::BGRA32:
                return 4;
            case PixelFormat::Alpha8:
                return 1;
            case PixelFormat::sRGB24:
                return 3;
            case PixelFormat::sRGBA32:
                return 4;
            default:
                LOG_WARN(logger, "Unknown texture channel number fallback to 0");
                break;
        }

        return 0;
    }
}  // namespace GLaDOS