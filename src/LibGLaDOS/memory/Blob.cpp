#include "Blob.h"

#include "math/Color.h"
#include "math/Quat.h"
#include "math/Vec2.h"
#include "math/Vec3.h"
#include "math/Vec4.h"
#include "math/Mat4.hpp"
#include "math/Point.hpp"
#include "math/Size.hpp"
#include "math/Rect.hpp"

namespace GLaDOS {
    Logger* Blob::logger = LoggerRegistry::getInstance().makeAndGetLogger("Blob");
    Blob::Blob(const std::byte* data, std::size_t size) {
        resize(size);
        copyFrom(data, size);
    }

    Blob& Blob::operator<<(int8_t value) {
        writeBytes(reinterpret_cast<std::byte*>(&value), sizeof(value));
        return *this;
    }

    Blob& Blob::operator<<(int16_t value) {
        writeBytes(reinterpret_cast<std::byte*>(&value), sizeof(value));
        return *this;
    }

    Blob& Blob::operator<<(int32_t value) {
        writeBytes(reinterpret_cast<std::byte*>(&value), sizeof(value));
        return *this;
    }

    Blob& Blob::operator<<(int64_t value) {
        writeBytes(reinterpret_cast<std::byte*>(&value), sizeof(value));
        return *this;
    }

    Blob& Blob::operator<<(uint8_t value) {
        writeBytes(reinterpret_cast<std::byte*>(&value), sizeof(value));
        return *this;
    }

    Blob& Blob::operator<<(uint16_t value) {
        writeBytes(reinterpret_cast<std::byte*>(&value), sizeof(value));
        return *this;
    }

    Blob& Blob::operator<<(uint32_t value) {
        writeBytes(reinterpret_cast<std::byte*>(&value), sizeof(value));
        return *this;
    }

    Blob& Blob::operator<<(uint64_t value) {
        writeBytes(reinterpret_cast<std::byte*>(&value), sizeof(value));
        return *this;
    }

    Blob& Blob::operator<<(float value) {
        writeBytes(reinterpret_cast<std::byte*>(&value), sizeof(value));
        return *this;
    }

    Blob& Blob::operator<<(double value) {
        writeBytes(reinterpret_cast<std::byte*>(&value), sizeof(value));
        return *this;
    }

    Blob& Blob::operator<<(Vec2& value) {
        writeBytes(reinterpret_cast<std::byte*>(&value[0]), sizeof(value));
        return *this;
    }

    Blob& Blob::operator<<(Point<real>& value) {
        writeBytes(reinterpret_cast<std::byte*>(&value[0]), sizeof(value));
        return *this;
    }

    Blob& Blob::operator<<(Vec3& value) {
        writeBytes(reinterpret_cast<std::byte*>(&value[0]), sizeof(value));
        return *this;
    }

    Blob& Blob::operator<<(Vec4& value) {
        writeBytes(reinterpret_cast<std::byte*>(&value[0]), sizeof(value));
        return *this;
    }

    Blob& Blob::operator<<(Quat& value) {
        writeBytes(reinterpret_cast<std::byte*>(&value[0]), sizeof(value));
        return *this;
    }

    Blob& Blob::operator<<(Color& value) {
        writeBytes(reinterpret_cast<std::byte*>(&value[0]), sizeof(value));
        return *this;
    }

    Blob& Blob::operator<<(Point<int32_t>& value) {
        writeBytes(reinterpret_cast<std::byte*>(&value[0]), sizeof(value));
        return *this;
    }

    Blob& Blob::operator<<(Size<int32_t>& value) {
        writeBytes(reinterpret_cast<std::byte*>(&value.w), sizeof(value));
        return *this;
    }

    Blob& Blob::operator<<(Rect<int32_t>& value) {
        writeBytes(reinterpret_cast<std::byte*>(&value.x), sizeof(value));
        return *this;
    }

    Blob& Blob::operator<<(Point<uint32_t>& value) {
        writeBytes(reinterpret_cast<std::byte*>(&value[0]), sizeof(value));
        return *this;
    }

    Blob& Blob::operator<<(Size<uint32_t>& value) {
        writeBytes(reinterpret_cast<std::byte*>(&value.w), sizeof(value));
        return *this;
    }

    Blob& Blob::operator<<(Rect<uint32_t>& value) {
        writeBytes(reinterpret_cast<std::byte*>(&value.x), sizeof(value));
        return *this;
    }

    Blob& Blob::operator<<(Mat4<real>& value) {
        writeBytes(reinterpret_cast<std::byte*>(value.pointer()), value.size());
        return *this;
    }

    void Blob::copyFrom(Blob& buffer) {
        throwIfOverflow(buffer.size());
        std::copy(buffer.pointer(), buffer.pointer() + buffer.size(), pointer());
    }

    void Blob::copyFrom(const Vector<std::byte>& data) {
        throwIfOverflow(data.size());
        std::copy(data.data(), data.data() + data.size(), pointer());
    }

    void Blob::copyFrom(const std::byte* data, size_t size) {
        throwIfOverflow(size);
        std::copy(data, data + size, pointer());
    }

    void Blob::copyFrom(std::size_t offset, const std::byte* data, std::size_t size) {
        throwIfOverflow(size);
        std::copy(data, data + size, pointer() + offset);
    }

    void Blob::insertFrom(Blob& buffer) {
        std::copy(buffer.pointer(), buffer.pointer() + buffer.size(), std::back_inserter(mData));
    }

    void Blob::insertFrom(const Vector<std::byte>& data) {
        std::copy(data.data(), data.data() + data.size(), std::back_inserter(mData));
    }

    void Blob::insertFrom(const std::byte* data, std::size_t size) {
        std::copy(data, data + size, std::back_inserter(mData));
    }

    std::byte* Blob::pointer() {
        return mData.data();
    }

    void const* Blob::constPointer() const {
        return &mData.front();
    }

    std::size_t Blob::size() const {
        return mData.size();
    }

    void Blob::resize(std::size_t n) {
        mData.resize(n);
    }

    void Blob::clear() {
        mData.clear();
    }

    bool Blob::isEmpty() const {
        return mData.empty();
    }

    void Blob::throwIfOverflow(std::size_t size) const {
        if (this->size() < size) {
            LOG_ERROR(logger, "Range overflow {0} > {1}", size, this->size());
            throw std::runtime_error(
                "Range overflow" + std::to_string(size) + " > " + std::to_string(this->size()));
        }
    }

    void Blob::writeBytes(std::byte* bytes, std::size_t count) {
        if (size() < count) {
            LOG_ERROR(logger, "buffer overflow");
            return;
        }

        std::copy(bytes, bytes + count, pointer());
    }
}  // namespace GLaDOS