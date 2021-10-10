#include "VertexData.h"

#include "VertexFormat.h"

namespace GLaDOS {
    VertexData::VertexData(const VertexFormatDescriptor& vertexFormatDescriptor, std::size_t count, bool allocate) {
        mVertexFormatHolder = vertexFormatDescriptor.makeVertexFormatHolder();
        mVertexFormatDescriptor = vertexFormatDescriptor;  // should be assigned after makeVertexFormatHolder() call
        for (const auto& vertexFormat : *mVertexFormatHolder) {
            mStride += vertexFormat->size();
        }

        mSize = count * mStride;

        if (allocate) {
            this->allocate();
        }
    }

    VertexData::~VertexData() {
        DELETE_T(mVertexFormatHolder, VertexFormatHolder);
    }

    VertexFormatHolder* VertexData::getVertexFormatHolder() const {
        return mVertexFormatHolder;
    }

    Vec3 VertexData::getPosition(std::size_t index) {
        assert(mVertexFormatDescriptor.mUsePosition);
        return *reinterpret_cast<Vec3*>(static_cast<char*>(mBufferData) + calcOffset(index, mVertexFormatDescriptor.mPositionOffset));
    }

    void VertexData::setPosition(std::size_t index, const Vec3& position) {
        assert(mVertexFormatDescriptor.mUsePosition);
        *reinterpret_cast<Vec3*>(static_cast<char*>(mBufferData) + calcOffset(index, mVertexFormatDescriptor.mPositionOffset)) = position;
    }

    Vec3 VertexData::getNormal(std::size_t index) {
        assert(mVertexFormatDescriptor.mUseNormal);
        return *reinterpret_cast<Vec3*>(static_cast<char*>(mBufferData) + calcOffset(index, mVertexFormatDescriptor.mNormalOffset));
    }

    void VertexData::setNormal(std::size_t index, const Vec3& normal) {
        assert(mVertexFormatDescriptor.mUseNormal);
        *reinterpret_cast<Vec3*>(static_cast<char*>(mBufferData) + calcOffset(index, mVertexFormatDescriptor.mNormalOffset)) = normal;
    }

    Vec2 VertexData::getTexCoord0(std::size_t index) {
        assert(mVertexFormatDescriptor.mUseTexCoord0);
        return *reinterpret_cast<Vec2*>(static_cast<char*>(mBufferData) + calcOffset(index, mVertexFormatDescriptor.mTexCoord0Offset));
    }

    void VertexData::setTexCoord0(std::size_t index, const Vec2& texCoord) {
        assert(mVertexFormatDescriptor.mUseTexCoord0);
        *reinterpret_cast<Vec2*>(static_cast<char*>(mBufferData) + calcOffset(index, mVertexFormatDescriptor.mTexCoord0Offset)) = texCoord;
    }

    Vec2 VertexData::getTexCoord1(std::size_t index) {
        assert(mVertexFormatDescriptor.mUseTexCoord1);
        return *reinterpret_cast<Vec2*>(static_cast<char*>(mBufferData) + calcOffset(index, mVertexFormatDescriptor.mTexCoord1Offset));
    }

    void VertexData::setTexCoord1(std::size_t index, const Vec2& texCoord) {
        assert(mVertexFormatDescriptor.mUseTexCoord1);
        *reinterpret_cast<Vec2*>(static_cast<char*>(mBufferData) + calcOffset(index, mVertexFormatDescriptor.mTexCoord1Offset)) = texCoord;
    }

    Vec2 VertexData::getTexCoord2(std::size_t index) {
        assert(mVertexFormatDescriptor.mUseTexCoord2);
        return *reinterpret_cast<Vec2*>(static_cast<char*>(mBufferData) + calcOffset(index, mVertexFormatDescriptor.mTexCoord2Offset));
    }

    void VertexData::setTexCoord2(std::size_t index, const Vec2& texCoord) {
        assert(mVertexFormatDescriptor.mUseTexCoord2);
        *reinterpret_cast<Vec2*>(static_cast<char*>(mBufferData) + calcOffset(index, mVertexFormatDescriptor.mTexCoord2Offset)) = texCoord;
    }

    Vec2 VertexData::getTexCoord3(std::size_t index) {
        assert(mVertexFormatDescriptor.mUseTexCoord3);
        return *reinterpret_cast<Vec2*>(static_cast<char*>(mBufferData) + calcOffset(index, mVertexFormatDescriptor.mTexCoord3Offset));
    }

    void VertexData::setTexCoord3(std::size_t index, const Vec2& texCoord) {
        assert(mVertexFormatDescriptor.mUseTexCoord3);
        *reinterpret_cast<Vec2*>(static_cast<char*>(mBufferData) + calcOffset(index, mVertexFormatDescriptor.mTexCoord3Offset)) = texCoord;
    }

    Vec2 VertexData::getTexCoord4(std::size_t index) {
        assert(mVertexFormatDescriptor.mUseTexCoord4);
        return *reinterpret_cast<Vec2*>(static_cast<char*>(mBufferData) + calcOffset(index, mVertexFormatDescriptor.mTexCoord4Offset));
    }

    void VertexData::setTexCoord4(std::size_t index, const Vec2& texCoord) {
        assert(mVertexFormatDescriptor.mUseTexCoord4);
        *reinterpret_cast<Vec2*>(static_cast<char*>(mBufferData) + calcOffset(index, mVertexFormatDescriptor.mTexCoord4Offset)) = texCoord;
    }

    Vec2 VertexData::getTexCoord5(std::size_t index) {
        assert(mVertexFormatDescriptor.mUseTexCoord5);
        return *reinterpret_cast<Vec2*>(static_cast<char*>(mBufferData) + calcOffset(index, mVertexFormatDescriptor.mTexCoord5Offset));
    }

    void VertexData::setTexCoord5(std::size_t index, const Vec2& texCoord) {
        assert(mVertexFormatDescriptor.mUseTexCoord5);
        *reinterpret_cast<Vec2*>(static_cast<char*>(mBufferData) + calcOffset(index, mVertexFormatDescriptor.mTexCoord5Offset)) = texCoord;
    }

    Vec2 VertexData::getTexCoord6(std::size_t index) {
        assert(mVertexFormatDescriptor.mUseTexCoord6);
        return *reinterpret_cast<Vec2*>(static_cast<char*>(mBufferData) + calcOffset(index, mVertexFormatDescriptor.mTexCoord6Offset));
    }

    void VertexData::setTexCoord6(std::size_t index, const Vec2& texCoord) {
        assert(mVertexFormatDescriptor.mUseTexCoord6);
        *reinterpret_cast<Vec2*>(static_cast<char*>(mBufferData) + calcOffset(index, mVertexFormatDescriptor.mTexCoord6Offset)) = texCoord;
    }

    Vec2 VertexData::getTexCoord7(std::size_t index) {
        assert(mVertexFormatDescriptor.mUseTexCoord7);
        return *reinterpret_cast<Vec2*>(static_cast<char*>(mBufferData) + calcOffset(index, mVertexFormatDescriptor.mTexCoord7Offset));
    }

    void VertexData::setTexCoord7(std::size_t index, const Vec2& texCoord) {
        assert(mVertexFormatDescriptor.mUseTexCoord7);
        *reinterpret_cast<Vec2*>(static_cast<char*>(mBufferData) + calcOffset(index, mVertexFormatDescriptor.mTexCoord7Offset)) = texCoord;
    }

    Color VertexData::getColor(std::size_t index) {
        assert(mVertexFormatDescriptor.mUseColor);
        return *reinterpret_cast<Color*>(static_cast<char*>(mBufferData) + calcOffset(index, mVertexFormatDescriptor.mColorOffset));
    }

    void VertexData::setColor(std::size_t index, const Color& color) {
        assert(mVertexFormatDescriptor.mUseColor);
        *reinterpret_cast<Color*>(static_cast<char*>(mBufferData) + calcOffset(index, mVertexFormatDescriptor.mColorOffset)) = color;
    }

    void VertexData::throwIfOverflow(std::size_t size) const {
        if (mSize < size) {
            throw std::runtime_error(
                "Range overflow" + std::to_string(size) + " > " + std::to_string(mSize));
        }
    }

    std::size_t VertexData::calcOffset(std::size_t index, std::size_t offset) const {
        std::size_t calculatedOffset = index * mStride + offset;
        throwIfOverflow(calculatedOffset);
        return calculatedOffset;
    }
}  // namespace GLaDOS