#ifndef GLADOS_VERTEXFORMAT_H
#define GLADOS_VERTEXFORMAT_H

#include "utils/Enumeration.h"
#include "utils/Utility.h"

namespace GLaDOS {
    class Logger;
    class VertexFormat {
      public:
        explicit VertexFormat(VertexSemantic _semantic, VertexAttributeType _attributeType);
        VertexSemantic semantic() const;
        VertexAttributeType type() const;

        std::size_t size() const;
        std::size_t sizeAlign4() const;

      private:
        static Logger* logger;

        VertexSemantic mSemantic{VertexSemantic::Unknown};
        VertexAttributeType mType{VertexAttributeType::Unknown};
    };

    class VertexFormatHolder {
      public:
        typedef Vector<VertexFormat*>::iterator iterator;
        typedef const Vector<VertexFormat*>::const_iterator const_iterator;

        VertexFormatHolder(Vector<VertexFormat*>& vertexFormats);
        ~VertexFormatHolder();

        iterator begin();
        const_iterator begin() const;
        iterator end();
        const_iterator end() const;
        Vector<VertexFormat*> getVertexFormats() const;

      private:
        Vector<VertexFormat*> mVertexFormats;
    };

    class VertexFormatDescriptor {
        friend class VertexData;

      public:
        VertexFormatDescriptor() = default;
        ~VertexFormatDescriptor() = default;

        VertexFormatDescriptor position();
        VertexFormatDescriptor normal();
        VertexFormatDescriptor texCoord0();
        VertexFormatDescriptor texCoord1();
        VertexFormatDescriptor texCoord2();
        VertexFormatDescriptor texCoord3();
        VertexFormatDescriptor texCoord4();
        VertexFormatDescriptor texCoord5();
        VertexFormatDescriptor texCoord6();
        VertexFormatDescriptor texCoord7();
        VertexFormatDescriptor color();
        VertexFormatHolder* makeVertexFormatHolder() const;

      private:
        static std::size_t sumOfPreviousOffset(const Vector<VertexFormat*>& vertexFormats);

        bool mUsePosition{false};
        mutable std::size_t mPositionOffset{0};
        bool mUseNormal{false};
        mutable std::size_t mNormalOffset{0};
        bool mUseTexCoord0{false};
        mutable std::size_t mTexCoord0Offset{0};
        bool mUseTexCoord1{false};
        mutable std::size_t mTexCoord1Offset{0};
        bool mUseTexCoord2{false};
        mutable std::size_t mTexCoord2Offset{0};
        bool mUseTexCoord3{false};
        mutable std::size_t mTexCoord3Offset{0};
        bool mUseTexCoord4{false};
        mutable std::size_t mTexCoord4Offset{0};
        bool mUseTexCoord5{false};
        mutable std::size_t mTexCoord5Offset{0};
        bool mUseTexCoord6{false};
        mutable std::size_t mTexCoord6Offset{0};
        bool mUseTexCoord7{false};
        mutable std::size_t mTexCoord7Offset{0};
        bool mUseColor{false};
        mutable std::size_t mColorOffset{0};
    };
}  // namespace GLaDOS

#endif  //GLADOS_VERTEXFORMAT_H
