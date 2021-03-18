#include "VertexData.h"

namespace GLaDOS {
  VertexData::VertexData(const VertexFormatBuilder& vertexFormatBuilder, std::size_t count) {
    mVertexFormats = vertexFormatBuilder.build();
    mBuilder = vertexFormatBuilder;  // should be assigned after build call
    for (const auto& vertexFormat : mVertexFormats) {
      mStride += vertexFormat->size();
    }

    mStreamBuffer.resize(count * mStride);
  }

  VertexData::~VertexData() {
    deallocIterable(mVertexFormats);
  }

  Vector<VertexFormat*> VertexData::getVertexFormats() const {
    return mVertexFormats;
  }

  Vec3 VertexData::getPosition(std::size_t index) {
    assert(mBuilder.mUsePosition);
    return *static_cast<Vec3*>(mStreamBuffer.offsetOf(index * mStride + mBuilder.mPositionOffset));
  }

  void VertexData::setPosition(std::size_t index, const Vec3& position) {
    assert(mBuilder.mUsePosition);
    *static_cast<Vec3*>(mStreamBuffer.offsetOf(index * mStride + mBuilder.mPositionOffset)) = position;
  }

  Vec3 VertexData::getNormal(std::size_t index) {
    assert(mBuilder.mUseNormal);
    return *static_cast<Vec3*>(mStreamBuffer.offsetOf(index * mStride + mBuilder.mNormalOffset));
  }

  void VertexData::setNormal(std::size_t index, const Vec3& normal) {
    assert(mBuilder.mUseNormal);
    *static_cast<Vec3*>(mStreamBuffer.offsetOf(index * mStride + mBuilder.mNormalOffset)) = normal;
  }

  Vec2 VertexData::getTexCoord0(std::size_t index) {
    assert(mBuilder.mUseTexCoord0);
    return *static_cast<Vec2*>(mStreamBuffer.offsetOf(index * mStride + mBuilder.mTexCoord0Offset));
  }

  void VertexData::setTexCoord0(std::size_t index, const Vec2& texCoord) {
    assert(mBuilder.mUseTexCoord0);
    *static_cast<Vec2*>(mStreamBuffer.offsetOf(index * mStride + mBuilder.mTexCoord0Offset)) = texCoord;
  }

  Vec2 VertexData::getTexCoord1(std::size_t index) {
    assert(mBuilder.mUseTexCoord1);
    return *static_cast<Vec2*>(mStreamBuffer.offsetOf(index * mStride + mBuilder.mTexCoord1Offset));
  }

  void VertexData::setTexCoord1(std::size_t index, const Vec2& texCoord) {
    assert(mBuilder.mUseTexCoord1);
    *static_cast<Vec2*>(mStreamBuffer.offsetOf(index * mStride + mBuilder.mTexCoord1Offset)) = texCoord;
  }

  Vec2 VertexData::getTexCoord2(std::size_t index) {
    assert(mBuilder.mUseTexCoord2);
    return *static_cast<Vec2*>(mStreamBuffer.offsetOf(index * mStride + mBuilder.mTexCoord2Offset));
  }

  void VertexData::setTexCoord2(std::size_t index, const Vec2& texCoord) {
    assert(mBuilder.mUseTexCoord2);
    *static_cast<Vec2*>(mStreamBuffer.offsetOf(index * mStride + mBuilder.mTexCoord2Offset)) = texCoord;
  }

  Vec2 VertexData::getTexCoord3(std::size_t index) {
    assert(mBuilder.mUseTexCoord3);
    return *static_cast<Vec2*>(mStreamBuffer.offsetOf(index * mStride + mBuilder.mTexCoord3Offset));
  }

  void VertexData::setTexCoord3(std::size_t index, const Vec2& texCoord) {
    assert(mBuilder.mUseTexCoord3);
    *static_cast<Vec2*>(mStreamBuffer.offsetOf(index * mStride + mBuilder.mTexCoord3Offset)) = texCoord;
  }

  Vec2 VertexData::getTexCoord4(std::size_t index) {
    assert(mBuilder.mUseTexCoord4);
    return *static_cast<Vec2*>(mStreamBuffer.offsetOf(index * mStride + mBuilder.mTexCoord4Offset));
  }

  void VertexData::setTexCoord4(std::size_t index, const Vec2& texCoord) {
    assert(mBuilder.mUseTexCoord4);
    *static_cast<Vec2*>(mStreamBuffer.offsetOf(index * mStride + mBuilder.mTexCoord4Offset)) = texCoord;
  }

  Vec2 VertexData::getTexCoord5(std::size_t index) {
    assert(mBuilder.mUseTexCoord5);
    return *static_cast<Vec2*>(mStreamBuffer.offsetOf(index * mStride + mBuilder.mTexCoord5Offset));
  }

  void VertexData::setTexCoord5(std::size_t index, const Vec2& texCoord) {
    assert(mBuilder.mUseTexCoord5);
    *static_cast<Vec2*>(mStreamBuffer.offsetOf(index * mStride + mBuilder.mTexCoord5Offset)) = texCoord;
  }

  Vec2 VertexData::getTexCoord6(std::size_t index) {
    assert(mBuilder.mUseTexCoord6);
    return *static_cast<Vec2*>(mStreamBuffer.offsetOf(index * mStride + mBuilder.mTexCoord6Offset));
  }

  void VertexData::setTexCoord6(std::size_t index, const Vec2& texCoord) {
    assert(mBuilder.mUseTexCoord6);
    *static_cast<Vec2*>(mStreamBuffer.offsetOf(index * mStride + mBuilder.mTexCoord6Offset)) = texCoord;
  }

  Vec2 VertexData::getTexCoord7(std::size_t index) {
    assert(mBuilder.mUseTexCoord7);
    return *static_cast<Vec2*>(mStreamBuffer.offsetOf(index * mStride + mBuilder.mTexCoord7Offset));
  }

  void VertexData::setTexCoord7(std::size_t index, const Vec2& texCoord) {
    assert(mBuilder.mUseTexCoord7);
    *static_cast<Vec2*>(mStreamBuffer.offsetOf(index * mStride + mBuilder.mTexCoord7Offset)) = texCoord;
  }

  Color VertexData::getColor(std::size_t index) {
    assert(mBuilder.mUseColor);
    return *static_cast<Color*>(mStreamBuffer.offsetOf(index * mStride + mBuilder.mColorOffset));
  }

  void VertexData::setColor(std::size_t index, const Color& color) {
    assert(mBuilder.mUseColor);
    *static_cast<Color*>(mStreamBuffer.offsetOf(index * mStride + mBuilder.mColorOffset)) = color;
  }
}  // namespace GLaDOS