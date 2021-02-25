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

  Vec4 VertexData::getPosition(std::size_t index) {
    assert(mBuilder.mUsePosition);
    return *static_cast<Vec4*>(mStreamBuffer.offsetOf(index * mStride + mBuilder.mPositionOffset));
  }

  void VertexData::setPosition(std::size_t index, const Vec4& position) {
    assert(mBuilder.mUsePosition);
    *static_cast<Vec4*>(mStreamBuffer.offsetOf(index * mStride + mBuilder.mPositionOffset)) = position;
  }

  Vec3 VertexData::getNormal(std::size_t index) {
    assert(mBuilder.mUseNormal);
    return *static_cast<Vec3*>(mStreamBuffer.offsetOf(index * mStride + mBuilder.mNormalOffset));
  }

  void VertexData::setNormal(std::size_t index, const Vec3& normal) {
    assert(mBuilder.mUseNormal);
    *static_cast<Vec3*>(mStreamBuffer.offsetOf(index * mStride + mBuilder.mNormalOffset)) = normal;
  }

  Vec2 VertexData::getDiffuseTexCoord(std::size_t index) {
    assert(mBuilder.mUseDiffuseTexCoord);
    return *static_cast<Vec2*>(mStreamBuffer.offsetOf(index * mStride + mBuilder.mDiffuseTexCoordOffset));
  }

  void VertexData::setDiffuseTexCoord(std::size_t index, const Vec2& texCoord) {
    assert(mBuilder.mUseDiffuseTexCoord);
    *static_cast<Vec2*>(mStreamBuffer.offsetOf(index * mStride + mBuilder.mDiffuseTexCoordOffset)) = texCoord;
  }

  Vec2 VertexData::getReflectionTexCoord(std::size_t index) {
    assert(mBuilder.mUseReflectionTexCoord);
    return *static_cast<Vec2*>(mStreamBuffer.offsetOf(index * mStride + mBuilder.mReflectionTexCoordOffset));
  }

  void VertexData::setReflectionTexCoord(std::size_t index, const Vec2& texCoord) {
    assert(mBuilder.mUseReflectionTexCoord);
    *static_cast<Vec2*>(mStreamBuffer.offsetOf(index * mStride + mBuilder.mReflectionTexCoordOffset)) = texCoord;
  }

  Vec2 VertexData::getNormalTexCoord(std::size_t index) {
    assert(mBuilder.mUseNormalTexCoord);
    return *static_cast<Vec2*>(mStreamBuffer.offsetOf(index * mStride + mBuilder.mNormalTexCoordOffset));
  }

  void VertexData::setNormalTexCoord(std::size_t index, const Vec2& texCoord) {
    assert(mBuilder.mUseNormalTexCoord);
    *static_cast<Vec2*>(mStreamBuffer.offsetOf(index * mStride + mBuilder.mNormalTexCoordOffset)) = texCoord;
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