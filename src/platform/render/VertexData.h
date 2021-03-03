#ifndef GLADOS_VERTEXDATA_H
#define GLADOS_VERTEXDATA_H

#include "ByteBuffer.h"
#include "VertexFormat.h"
#include "math/Color.h"
#include "math/Vec2.h"
#include "math/Vec3.h"

namespace GLaDOS {
  class VertexFormat;
  class VertexData : public ByteBuffer {
  public:
    explicit VertexData(const VertexFormatBuilder& vertexFormatBuilder, std::size_t count);
    ~VertexData() override;

    Vector<VertexFormat*> getVertexFormats() const;

    Vec4 getPosition(std::size_t index);
    void setPosition(std::size_t index, const Vec4& position);
    Vec3 getNormal(std::size_t index);
    void setNormal(std::size_t index, const Vec3& normal);
    Vec2 getDiffuseTexCoord(std::size_t index);
    void setDiffuseTexCoord(std::size_t index, const Vec2& texCoord);
    Vec2 getReflectionTexCoord(std::size_t index);
    void setReflectionTexCoord(std::size_t index, const Vec2& texCoord);
    Vec2 getNormalTexCoord(std::size_t index);
    void setNormalTexCoord(std::size_t index, const Vec2& texCoord);
    Color getColor(std::size_t index);
    void setColor(std::size_t index, const Color& color);

  private:
    VertexFormatBuilder mBuilder;
    Vector<VertexFormat*> mVertexFormats;
  };
}  // namespace GLaDOS

#endif  //GLADOS_VERTEXDATA_H
