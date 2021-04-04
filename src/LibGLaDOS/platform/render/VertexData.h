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
    explicit VertexData(const VertexFormatBuilder& vertexFormatBuilder, std::size_t count, bool allocate = false);
    ~VertexData() override;

    Vector<VertexFormat*> getVertexFormats() const;

    Vec3 getPosition(std::size_t index);
    void setPosition(std::size_t index, const Vec3& position);
    Vec3 getNormal(std::size_t index);
    void setNormal(std::size_t index, const Vec3& normal);
    Vec2 getTexCoord0(std::size_t index);
    void setTexCoord0(std::size_t index, const Vec2& texCoord);
    Vec2 getTexCoord1(std::size_t index);
    void setTexCoord1(std::size_t index, const Vec2& texCoord);
    Vec2 getTexCoord2(std::size_t index);
    void setTexCoord2(std::size_t index, const Vec2& texCoord);
    Vec2 getTexCoord3(std::size_t index);
    void setTexCoord3(std::size_t index, const Vec2& texCoord);
    Vec2 getTexCoord4(std::size_t index);
    void setTexCoord4(std::size_t index, const Vec2& texCoord);
    Vec2 getTexCoord5(std::size_t index);
    void setTexCoord5(std::size_t index, const Vec2& texCoord);
    Vec2 getTexCoord6(std::size_t index);
    void setTexCoord6(std::size_t index, const Vec2& texCoord);
    Vec2 getTexCoord7(std::size_t index);
    void setTexCoord7(std::size_t index, const Vec2& texCoord);
    Color getColor(std::size_t index);
    void setColor(std::size_t index, const Color& color);

  protected:
    void throwIfOverflow(std::size_t size) const;

  private:
    VertexFormatBuilder mBuilder;
    Vector<VertexFormat*> mVertexFormats;
  };
}  // namespace GLaDOS

#endif  //GLADOS_VERTEXDATA_H
