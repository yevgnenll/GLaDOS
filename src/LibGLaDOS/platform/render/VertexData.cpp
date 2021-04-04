#include "VertexData.h"

namespace GLaDOS {
  VertexData::VertexData(const VertexFormatBuilder& vertexFormatBuilder, std::size_t count, bool allocate) {
    mVertexFormats = vertexFormatBuilder.build();
    mBuilder = vertexFormatBuilder;  // should be assigned after build call
    for (const auto& vertexFormat : mVertexFormats) {
      mStride += vertexFormat->size();
    }

    mSize = count * mStride;

    if (allocate) {
      this->allocate();
    }
  }

  VertexData::~VertexData() {
    deallocIterable(mVertexFormats);
  }

  Vector<VertexFormat*> VertexData::getVertexFormats() const {
    return mVertexFormats;
  }

  Vec3 VertexData::getPosition(std::size_t index) {
    assert(mBuilder.mUsePosition);
    std::size_t offset = index * mStride + mBuilder.mPositionOffset;
    throwIfOverflow(offset);
    return *reinterpret_cast<Vec3*>(static_cast<char*>(mBufferData) + offset);
  }

  void VertexData::setPosition(std::size_t index, const Vec3& position) {
    assert(mBuilder.mUsePosition);
    std::size_t offset = index * mStride + mBuilder.mPositionOffset;
    throwIfOverflow(offset);
    *reinterpret_cast<Vec3*>(static_cast<char*>(mBufferData) + offset) = position;
  }

  Vec3 VertexData::getNormal(std::size_t index) {
    assert(mBuilder.mUseNormal);
    std::size_t offset = index * mStride + mBuilder.mNormalOffset;
    throwIfOverflow(offset);
    return *reinterpret_cast<Vec3*>(static_cast<char*>(mBufferData) + offset);
  }

  void VertexData::setNormal(std::size_t index, const Vec3& normal) {
    assert(mBuilder.mUseNormal);
    std::size_t offset = index * mStride + mBuilder.mNormalOffset;
    throwIfOverflow(offset);
    *reinterpret_cast<Vec3*>(static_cast<char*>(mBufferData) + offset) = normal;
  }

  Vec2 VertexData::getTexCoord0(std::size_t index) {
    assert(mBuilder.mUseTexCoord0);
    std::size_t offset = index * mStride + mBuilder.mTexCoord0Offset;
    throwIfOverflow(offset);
    return *reinterpret_cast<Vec2*>(static_cast<char*>(mBufferData) + offset);
  }

  void VertexData::setTexCoord0(std::size_t index, const Vec2& texCoord) {
    assert(mBuilder.mUseTexCoord0);
    std::size_t offset = index * mStride + mBuilder.mTexCoord0Offset;
    throwIfOverflow(offset);
    *reinterpret_cast<Vec2*>(static_cast<char*>(mBufferData) + offset) = texCoord;
  }

  Vec2 VertexData::getTexCoord1(std::size_t index) {
    assert(mBuilder.mUseTexCoord1);
    std::size_t offset = index * mStride + mBuilder.mTexCoord1Offset;
    throwIfOverflow(offset);
    return *reinterpret_cast<Vec2*>(static_cast<char*>(mBufferData) + offset);
  }

  void VertexData::setTexCoord1(std::size_t index, const Vec2& texCoord) {
    assert(mBuilder.mUseTexCoord1);
    std::size_t offset = index * mStride + mBuilder.mTexCoord1Offset;
    throwIfOverflow(offset);
    *reinterpret_cast<Vec2*>(static_cast<char*>(mBufferData) + offset) = texCoord;
  }

  Vec2 VertexData::getTexCoord2(std::size_t index) {
    assert(mBuilder.mUseTexCoord2);
    std::size_t offset = index * mStride + mBuilder.mTexCoord2Offset;
    throwIfOverflow(offset);
    return *reinterpret_cast<Vec2*>(static_cast<char*>(mBufferData) + offset);
  }

  void VertexData::setTexCoord2(std::size_t index, const Vec2& texCoord) {
    assert(mBuilder.mUseTexCoord2);
    std::size_t offset = index * mStride + mBuilder.mTexCoord2Offset;
    throwIfOverflow(offset);
    *reinterpret_cast<Vec2*>(static_cast<char*>(mBufferData) + offset) = texCoord;
  }

  Vec2 VertexData::getTexCoord3(std::size_t index) {
    assert(mBuilder.mUseTexCoord3);
    std::size_t offset = index * mStride + mBuilder.mTexCoord3Offset;
    throwIfOverflow(offset);
    return *reinterpret_cast<Vec2*>(static_cast<char*>(mBufferData) + offset);
  }

  void VertexData::setTexCoord3(std::size_t index, const Vec2& texCoord) {
    assert(mBuilder.mUseTexCoord3);
    std::size_t offset = index * mStride + mBuilder.mTexCoord3Offset;
    throwIfOverflow(offset);
    *reinterpret_cast<Vec2*>(static_cast<char*>(mBufferData) + offset) = texCoord;
  }

  Vec2 VertexData::getTexCoord4(std::size_t index) {
    assert(mBuilder.mUseTexCoord4);
    std::size_t offset = index * mStride + mBuilder.mTexCoord4Offset;
    throwIfOverflow(offset);
    return *reinterpret_cast<Vec2*>(static_cast<char*>(mBufferData) + offset);
  }

  void VertexData::setTexCoord4(std::size_t index, const Vec2& texCoord) {
    assert(mBuilder.mUseTexCoord4);
    std::size_t offset = index * mStride + mBuilder.mTexCoord4Offset;
    throwIfOverflow(offset);
    *reinterpret_cast<Vec2*>(static_cast<char*>(mBufferData) + offset) = texCoord;
  }

  Vec2 VertexData::getTexCoord5(std::size_t index) {
    assert(mBuilder.mUseTexCoord5);
    std::size_t offset = index * mStride + mBuilder.mTexCoord5Offset;
    throwIfOverflow(offset);
    return *reinterpret_cast<Vec2*>(static_cast<char*>(mBufferData) + offset);
  }

  void VertexData::setTexCoord5(std::size_t index, const Vec2& texCoord) {
    assert(mBuilder.mUseTexCoord5);
    std::size_t offset = index * mStride + mBuilder.mTexCoord5Offset;
    throwIfOverflow(offset);
    *reinterpret_cast<Vec2*>(static_cast<char*>(mBufferData) + offset) = texCoord;
  }

  Vec2 VertexData::getTexCoord6(std::size_t index) {
    assert(mBuilder.mUseTexCoord6);
    std::size_t offset = index * mStride + mBuilder.mTexCoord6Offset;
    throwIfOverflow(offset);
    return *reinterpret_cast<Vec2*>(static_cast<char*>(mBufferData) + offset);
  }

  void VertexData::setTexCoord6(std::size_t index, const Vec2& texCoord) {
    assert(mBuilder.mUseTexCoord6);
    std::size_t offset = index * mStride + mBuilder.mTexCoord6Offset;
    throwIfOverflow(offset);
    *reinterpret_cast<Vec2*>(static_cast<char*>(mBufferData) + offset) = texCoord;
  }

  Vec2 VertexData::getTexCoord7(std::size_t index) {
    assert(mBuilder.mUseTexCoord7);
    std::size_t offset = index * mStride + mBuilder.mTexCoord7Offset;
    throwIfOverflow(offset);
    return *reinterpret_cast<Vec2*>(static_cast<char*>(mBufferData) + offset);
  }

  void VertexData::setTexCoord7(std::size_t index, const Vec2& texCoord) {
    assert(mBuilder.mUseTexCoord7);
    std::size_t offset = index * mStride + mBuilder.mTexCoord7Offset;
    throwIfOverflow(offset);
    *reinterpret_cast<Vec2*>(static_cast<char*>(mBufferData) + offset) = texCoord;
  }

  Color VertexData::getColor(std::size_t index) {
    assert(mBuilder.mUseColor);
    std::size_t offset = index * mStride + mBuilder.mColorOffset;
    throwIfOverflow(offset);
    return *reinterpret_cast<Color*>(static_cast<char*>(mBufferData) + offset);
  }

  void VertexData::setColor(std::size_t index, const Color& color) {
    assert(mBuilder.mUseColor);
    std::size_t offset = index * mStride + mBuilder.mColorOffset;
    throwIfOverflow(offset);
    *reinterpret_cast<Color*>(static_cast<char*>(mBufferData) + offset) = color;
  }

  void VertexData::throwIfOverflow(std::size_t size) const {
    if (mSize < size) {
      throw std::runtime_error(
          "Range overflow" + std::to_string(size) + " > " + std::to_string(mSize));
    }
  }
}  // namespace GLaDOS