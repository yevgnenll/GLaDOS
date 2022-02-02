#ifndef GLADOS_VERTEXBUFFER_H
#define GLADOS_VERTEXBUFFER_H

#include "UploadBuffer.h"
#include "VertexFormat.h"
#include "math/Color.h"
#include "math/Vec2.h"
#include "math/Vec3.h"

namespace GLaDOS {
    class VertexFormat;
    class VertexFormatHolder;
    class VertexBuffer : public UploadBuffer {
      public:
        explicit VertexBuffer(const VertexFormatDescriptor& vertexFormatBuilder, std::size_t count);
        ~VertexBuffer() override;

        VertexFormatHolder* getVertexFormatHolder() const;
        VertexFormatDescriptor getVertexFormatDescriptor() const;

        void copyBufferData(void *data) override;

        Vec3 getPosition(std::size_t index);
        void setPosition(std::size_t index, const Vec3& position);
        Vec3 getNormal(std::size_t index);
        void setNormal(std::size_t index, const Vec3& normal);
        Vec3 getTangent(std::size_t index);
        void setTangent(std::size_t index, const Vec3& tangent);
        Vec3 getBiTangent(std::size_t index);
        void setBiTangent(std::size_t index, const Vec3& biTangent);
        Vec4 getBoneWeight(std::size_t index);
        void setBoneWeight(std::size_t index, const Vec4& boneWeight);
        uint32_t* getBoneIndex(std::size_t index);
        void setBoneIndex(std::size_t index, uint32_t* boneIndex);
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
        std::size_t calcOffset(std::size_t index, std::size_t offset) const;

      private:
        VertexFormatDescriptor mVertexFormatDescriptor;
        VertexFormatHolder* mVertexFormatHolder;
    };
}  // namespace GLaDOS

#endif  // GLADOS_VERTEXBUFFER_H
