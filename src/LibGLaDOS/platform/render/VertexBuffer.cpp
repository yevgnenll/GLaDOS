#include "VertexBuffer.h"

#include "VertexFormat.h"

namespace GLaDOS {
    VertexBuffer::VertexBuffer(const VertexFormatDescriptor& vertexFormatDescriptor, std::size_t count) {
        mVertexFormatHolder = vertexFormatDescriptor.makeVertexFormatHolder();
        mVertexFormatDescriptor = vertexFormatDescriptor;  // should be assigned after makeVertexFormatHolder() call
        for (const auto& vertexFormat : *mVertexFormatHolder) {
            mStride += vertexFormat->size();
        }

        mSize = count * mStride;
        mBufferData.resize(mSize);
    }

    VertexBuffer::~VertexBuffer() {
        DELETE_T(mVertexFormatHolder, VertexFormatHolder);
    }

    VertexFormatHolder* VertexBuffer::getVertexFormatHolder() const {
        return mVertexFormatHolder;
    }

    VertexFormatDescriptor VertexBuffer::getVertexFormatDescriptor() const {
        return mVertexFormatDescriptor;
    }

    void VertexBuffer::copyBufferData(void* data) {
        mBufferData.copyFrom(reinterpret_cast<std::byte*>(data), mSize);
    }

    Vec3 VertexBuffer::getPosition(std::size_t index) {
        assert(mVertexFormatDescriptor.mUsePosition);
        std::size_t offset = calcOffset(index, mVertexFormatDescriptor.mPositionOffset);
        return *reinterpret_cast<Vec3*>(mBufferData.pointer() + offset);
    }

    void VertexBuffer::setPosition(std::size_t index, const Vec3& position) {
        assert(mVertexFormatDescriptor.mUsePosition);
        std::size_t offset = calcOffset(index, mVertexFormatDescriptor.mPositionOffset);
        mBufferData.copyFrom(offset, reinterpret_cast<const std::byte*>(&position), sizeof(position));
    }

    Vec3 VertexBuffer::getNormal(std::size_t index) {
        assert(mVertexFormatDescriptor.mUseNormal);
        std::size_t offset = calcOffset(index, mVertexFormatDescriptor.mNormalOffset);
        return *reinterpret_cast<Vec3*>(mBufferData.pointer() + offset);
    }

    void VertexBuffer::setNormal(std::size_t index, const Vec3& normal) {
        assert(mVertexFormatDescriptor.mUseNormal);
        std::size_t offset = calcOffset(index, mVertexFormatDescriptor.mNormalOffset);
        mBufferData.copyFrom(offset, reinterpret_cast<const std::byte*>(&normal), sizeof(normal));
    }

    Vec3 VertexBuffer::getTangent(std::size_t index) {
        assert(mVertexFormatDescriptor.mUseTangent);
        std::size_t offset = calcOffset(index, mVertexFormatDescriptor.mTangentOffset);
        return *reinterpret_cast<Vec3*>(mBufferData.pointer() + offset);
    }

    void VertexBuffer::setTangent(std::size_t index, const Vec3& tangent) {
        assert(mVertexFormatDescriptor.mUseTangent);
        std::size_t offset = calcOffset(index, mVertexFormatDescriptor.mNormalOffset);
        mBufferData.copyFrom(offset, reinterpret_cast<const std::byte*>(&tangent), sizeof(tangent));
    }

    Vec3 VertexBuffer::getBiTangent(std::size_t index) {
        assert(mVertexFormatDescriptor.mUseBiTangent);
        std::size_t offset = calcOffset(index, mVertexFormatDescriptor.mBiTangentOffset);
        return *reinterpret_cast<Vec3*>(mBufferData.pointer() + offset);
    }

    void VertexBuffer::setBiTangent(std::size_t index, const Vec3& biTangent) {
        assert(mVertexFormatDescriptor.mUseBiTangent);
        std::size_t offset = calcOffset(index, mVertexFormatDescriptor.mBiTangentOffset);
        mBufferData.copyFrom(offset, reinterpret_cast<const std::byte*>(&biTangent), sizeof(biTangent));
    }

    Vec4 VertexBuffer::getBoneWeight(std::size_t index) {
        assert(mVertexFormatDescriptor.mUseBoneWeight);
        std::size_t offset = calcOffset(index, mVertexFormatDescriptor.mBoneWeightOffset);
        return *reinterpret_cast<Vec4*>(mBufferData.pointer() + offset);
    }

    void VertexBuffer::setBoneWeight(std::size_t index, const Vec4& boneWeight) {
        assert(mVertexFormatDescriptor.mUseBoneWeight);
        std::size_t offset = calcOffset(index, mVertexFormatDescriptor.mBoneWeightOffset);
        mBufferData.copyFrom(offset, reinterpret_cast<const std::byte*>(&boneWeight), sizeof(boneWeight));
    }

    Array<uint8_t, 4> VertexBuffer::getBoneIndex(std::size_t index) {
        assert(mVertexFormatDescriptor.mUseBoneIndex);
        std::size_t offset = calcOffset(index, mVertexFormatDescriptor.mBoneIndexOffset);
        // unpack boneIndex from int32 to int8
        Array<uint8_t, 4> boneIndices;
        uint32_t packedInt = *reinterpret_cast<uint32_t*>(mBufferData.pointer() + offset);
        boneIndices[0] = static_cast<uint8_t>(packedInt & 0xFF);
        boneIndices[1] = static_cast<uint8_t>((packedInt >> 8) & 0xFF);
        boneIndices[2] = static_cast<uint8_t>((packedInt >> 16) & 0xFF);
        boneIndices[3] = static_cast<uint8_t>((packedInt >> 24) & 0xFF);
        return boneIndices;
    }

    void VertexBuffer::setBoneIndex(std::size_t index, uint8_t boneIndex0, uint8_t boneIndex1, uint8_t boneIndex2, uint8_t boneIndex3) {
        assert(mVertexFormatDescriptor.mUseBoneIndex);
        std::size_t offset = calcOffset(index, mVertexFormatDescriptor.mBoneIndexOffset);
        // pack boneIndex into int32
        uint32_t boneIndex = static_cast<uint32_t>(boneIndex0 | (boneIndex1 << 8) | (boneIndex2 << 16) | (boneIndex3 << 24));
        mBufferData.copyFrom(offset, reinterpret_cast<const std::byte*>(boneIndex), sizeof(uint32_t));
    }

    Vec2 VertexBuffer::getTexCoord0(std::size_t index) {
        assert(mVertexFormatDescriptor.mUseTexCoord0);
        std::size_t offset = calcOffset(index, mVertexFormatDescriptor.mTexCoord0Offset);
        return *reinterpret_cast<Vec2*>(mBufferData.pointer() + offset);
    }

    void VertexBuffer::setTexCoord0(std::size_t index, const Vec2& texCoord) {
        assert(mVertexFormatDescriptor.mUseTexCoord0);
        std::size_t offset = calcOffset(index, mVertexFormatDescriptor.mTexCoord0Offset);
        mBufferData.copyFrom(offset, reinterpret_cast<const std::byte*>(&texCoord), sizeof(texCoord));
    }

    Vec2 VertexBuffer::getTexCoord1(std::size_t index) {
        assert(mVertexFormatDescriptor.mUseTexCoord1);
        std::size_t offset = calcOffset(index, mVertexFormatDescriptor.mTexCoord1Offset);
        return *reinterpret_cast<Vec2*>(mBufferData.pointer() + offset);
    }

    void VertexBuffer::setTexCoord1(std::size_t index, const Vec2& texCoord) {
        assert(mVertexFormatDescriptor.mUseTexCoord1);
        std::size_t offset = calcOffset(index, mVertexFormatDescriptor.mTexCoord1Offset);
        mBufferData.copyFrom(offset, reinterpret_cast<const std::byte*>(&texCoord), sizeof(texCoord));
    }

    Vec2 VertexBuffer::getTexCoord2(std::size_t index) {
        assert(mVertexFormatDescriptor.mUseTexCoord2);
        std::size_t offset = calcOffset(index, mVertexFormatDescriptor.mTexCoord2Offset);
        return *reinterpret_cast<Vec2*>(mBufferData.pointer() + offset);
    }

    void VertexBuffer::setTexCoord2(std::size_t index, const Vec2& texCoord) {
        assert(mVertexFormatDescriptor.mUseTexCoord2);
        std::size_t offset = calcOffset(index, mVertexFormatDescriptor.mTexCoord2Offset);
        mBufferData.copyFrom(offset, reinterpret_cast<const std::byte*>(&texCoord), sizeof(texCoord));
    }

    Vec2 VertexBuffer::getTexCoord3(std::size_t index) {
        assert(mVertexFormatDescriptor.mUseTexCoord3);
        std::size_t offset = calcOffset(index, mVertexFormatDescriptor.mTexCoord3Offset);
        return *reinterpret_cast<Vec2*>(mBufferData.pointer() + offset);
    }

    void VertexBuffer::setTexCoord3(std::size_t index, const Vec2& texCoord) {
        assert(mVertexFormatDescriptor.mUseTexCoord3);
        std::size_t offset = calcOffset(index, mVertexFormatDescriptor.mTexCoord3Offset);
        mBufferData.copyFrom(offset, reinterpret_cast<const std::byte*>(&texCoord), sizeof(texCoord));
    }

    Vec2 VertexBuffer::getTexCoord4(std::size_t index) {
        assert(mVertexFormatDescriptor.mUseTexCoord4);
        std::size_t offset = calcOffset(index, mVertexFormatDescriptor.mTexCoord4Offset);
        return *reinterpret_cast<Vec2*>(mBufferData.pointer() + offset);
    }

    void VertexBuffer::setTexCoord4(std::size_t index, const Vec2& texCoord) {
        assert(mVertexFormatDescriptor.mUseTexCoord4);
        std::size_t offset = calcOffset(index, mVertexFormatDescriptor.mTexCoord4Offset);
        mBufferData.copyFrom(offset, reinterpret_cast<const std::byte*>(&texCoord), sizeof(texCoord));
    }

    Vec2 VertexBuffer::getTexCoord5(std::size_t index) {
        assert(mVertexFormatDescriptor.mUseTexCoord5);
        std::size_t offset = calcOffset(index, mVertexFormatDescriptor.mTexCoord5Offset);
        return *reinterpret_cast<Vec2*>(mBufferData.pointer() + offset);
    }

    void VertexBuffer::setTexCoord5(std::size_t index, const Vec2& texCoord) {
        assert(mVertexFormatDescriptor.mUseTexCoord5);
        std::size_t offset = calcOffset(index, mVertexFormatDescriptor.mTexCoord5Offset);
        mBufferData.copyFrom(offset, reinterpret_cast<const std::byte*>(&texCoord), sizeof(texCoord));
    }

    Vec2 VertexBuffer::getTexCoord6(std::size_t index) {
        assert(mVertexFormatDescriptor.mUseTexCoord6);
        std::size_t offset = calcOffset(index, mVertexFormatDescriptor.mTexCoord6Offset);
        return *reinterpret_cast<Vec2*>(mBufferData.pointer() + offset);
    }

    void VertexBuffer::setTexCoord6(std::size_t index, const Vec2& texCoord) {
        assert(mVertexFormatDescriptor.mUseTexCoord6);
        std::size_t offset = calcOffset(index, mVertexFormatDescriptor.mTexCoord6Offset);
        mBufferData.copyFrom(offset, reinterpret_cast<const std::byte*>(&texCoord), sizeof(texCoord));
    }

    Vec2 VertexBuffer::getTexCoord7(std::size_t index) {
        assert(mVertexFormatDescriptor.mUseTexCoord7);
        std::size_t offset = calcOffset(index, mVertexFormatDescriptor.mTexCoord7Offset);
        return *reinterpret_cast<Vec2*>(mBufferData.pointer() + offset);
    }

    void VertexBuffer::setTexCoord7(std::size_t index, const Vec2& texCoord) {
        assert(mVertexFormatDescriptor.mUseTexCoord7);
        std::size_t offset = calcOffset(index, mVertexFormatDescriptor.mTexCoord7Offset);
        mBufferData.copyFrom(offset, reinterpret_cast<const std::byte*>(&texCoord), sizeof(texCoord));
    }

    Color VertexBuffer::getColor(std::size_t index) {
        assert(mVertexFormatDescriptor.mUseColor);
        std::size_t offset = calcOffset(index, mVertexFormatDescriptor.mColorOffset);
        return *reinterpret_cast<Color*>(mBufferData.pointer() + offset);
    }

    void VertexBuffer::setColor(std::size_t index, const Color& color) {
        assert(mVertexFormatDescriptor.mUseColor);
        std::size_t offset = calcOffset(index, mVertexFormatDescriptor.mColorOffset);
        mBufferData.copyFrom(offset, reinterpret_cast<const std::byte*>(&color), sizeof(color));
    }

    void VertexBuffer::throwIfOverflow(std::size_t size) const {
        if (mSize < size) {
            throw std::runtime_error(
                "Range overflow" + std::to_string(size) + " > " + std::to_string(mSize));
        }
    }

    std::size_t VertexBuffer::calcOffset(std::size_t index, std::size_t offset) const {
        std::size_t calculatedOffset = index * mStride + offset;
        throwIfOverflow(calculatedOffset);
        return calculatedOffset;
    }
}  // namespace GLaDOS