#include "Texture3D.h"

namespace GLaDOS {
    Texture3D::Texture3D(const std::string& name, PixelFormat format) : Texture{name, format} {
        mDimension = TextureDimension::Tex3D;
    }

    Texture3D::~Texture3D() {
    }

    bool Texture3D::loadTextureFromFile() {
        return true;
    }

    bool Texture3D::loadTextureFromBuffer(StreamBuffer& buffer) {
        return true;
    }
}  // namespace GLaDOS