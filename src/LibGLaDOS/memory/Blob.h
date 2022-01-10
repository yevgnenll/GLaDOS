#ifndef GLADOS_BLOB_H
#define GLADOS_BLOB_H

#include "utils/Utility.h"

namespace GLaDOS {
    class Logger;
    class Vec2;
    class Vec3;
    class Vec4;
    class Quat;
    class Color;
    template <typename T>
    class Point;
    template <typename T>
    class Size;
    template <typename T>
    class Rect;
    template <typename T>
    class Mat4;
    class Blob {
      public:
        Blob() = default;
        Blob(const std::byte* data, std::size_t size);
        virtual ~Blob() = default;

        // all << operator behave backward emplacer.
        Blob& operator<<(int8_t value);
        Blob& operator<<(int16_t value);
        Blob& operator<<(int32_t value);
        Blob& operator<<(int64_t value);
        Blob& operator<<(uint8_t value);
        Blob& operator<<(uint16_t value);
        Blob& operator<<(uint32_t value);
        Blob& operator<<(uint64_t value);
        Blob& operator<<(float value);
        Blob& operator<<(double value);
        Blob& operator<<(Vec2& value);
        Blob& operator<<(Point<real>& value);
        Blob& operator<<(Vec3& value);
        Blob& operator<<(Vec4& value);
        Blob& operator<<(Quat& value);
        Blob& operator<<(Color& value);
        Blob& operator<<(Point<int32_t>& value);
        Blob& operator<<(Size<int32_t>& value);
        Blob& operator<<(Rect<int32_t>& value);
        Blob& operator<<(Point<uint32_t>& value);
        Blob& operator<<(Size<uint32_t>& value);
        Blob& operator<<(Rect<uint32_t>& value);
        Blob& operator<<(Mat4<real>& value);

        // NOTE: copyFrom 함수 호출전에 항상 resize로 버퍼를 확보해야함
        void copyFrom(Blob& buffer);
        void copyFrom(const Vector<std::byte>& data);
        void copyFrom(const std::byte* data, std::size_t size);
        void copyFrom(std::size_t offset, const std::byte* data, std::size_t size);

        // NOTE: insertFrom 함수는 기존 버퍼 뒤에 새로운 공간을 할당하여 데이터를 채워넣는다.
        void insertFrom(Blob& buffer);
        void insertFrom(const Vector<std::byte>& data);
        void insertFrom(const std::byte* data, std::size_t size);

        std::byte* pointer();
        const void* constPointer() const;
        std::size_t size() const;
        void resize(std::size_t n);
        void clear();
        bool isEmpty() const;

      protected:
        void throwIfOverflow(std::size_t size) const;

      private:
        void writeBytes(std::byte* bytes, std::size_t count);

        static Logger* logger;
        Vector<std::byte> mData;
    };
}  // namespace GLaDOS

#endif
