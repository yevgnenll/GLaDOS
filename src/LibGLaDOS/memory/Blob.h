#ifndef GLADOS_BLOB_H
#define GLADOS_BLOB_H

#include "math/Mat4.hpp"
#include "utils/Utility.h"

namespace GLaDOS {
    class Logger;
    class Vec2;
    class Vec3;
    class Vec4;
    class Quat;
    class Color;
    class Blob {
      public:
        Blob() = default;
        Blob(std::size_t size, void* data);
        virtual ~Blob() = default;

        Blob& operator<<(int8_t i);
        Blob& operator<<(int16_t i);
        Blob& operator<<(int32_t i);
        Blob& operator<<(int64_t i);
        Blob& operator<<(uint8_t i);
        Blob& operator<<(uint16_t i);
        Blob& operator<<(uint32_t i);
        Blob& operator<<(uint64_t i);
        Blob& operator<<(float i);
        Blob& operator<<(double i);
        Blob& operator<<(const Vec2& i);
        Blob& operator<<(const Vec3& i);
        Blob& operator<<(const Vec4& i);
        Blob& operator<<(const Quat& i);
        Blob& operator<<(const Color& i);
        template <typename T>
        Blob& operator<<(const Mat4<T>& i);

        void uploadData(Blob& buffer);
        void uploadData(const Vector<std::byte>& data);
        void uploadData(const std::byte* data, std::size_t size);

        void* offsetOf(std::size_t offset);
        void* pointer();
        const void* constPointer() const;
        std::size_t size() const;
        void resize(std::size_t n);
        void clear();
        bool isEmpty() const;

      protected:
        void throwIfOverflow(std::size_t size) const;

      private:
        void writeBytes(std::byte* bytes, unsigned int count);

        static Logger* logger;
        Vector<std::byte> mData;
    };

    template <typename T>
    Blob& Blob::operator<<(const Mat4<T>& i) {
        writeBytes(reinterpret_cast<std::byte*>(const_cast<real*>(i.pointer())), sizeof(T) * i.size());
        return *this;
    }
}  // namespace GLaDOS

#endif
