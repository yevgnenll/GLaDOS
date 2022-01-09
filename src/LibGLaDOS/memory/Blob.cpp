#include "Blob.h"

#include "math/Color.h"
#include "math/Quat.h"
#include "math/Vec2.h"
#include "math/Vec3.h"
#include "math/Vec4.h"

namespace GLaDOS {
    Logger* Blob::logger = LoggerRegistry::getInstance().makeAndGetLogger("Blob");
    Blob::Blob(const std::byte* data, std::size_t size) {
        resize(size);
        copyFrom(data, size);
    }

    Blob& Blob::operator<<(int8_t i) {
        writeBytes(reinterpret_cast<std::byte*>(&i), sizeof(i));
        return *this;
    }

    Blob& Blob::operator<<(int16_t i) {
        writeBytes(reinterpret_cast<std::byte*>(&i), sizeof(i));
        return *this;
    }

    Blob& Blob::operator<<(int32_t i) {
        writeBytes(reinterpret_cast<std::byte*>(&i), sizeof(i));
        return *this;
    }

    Blob& Blob::operator<<(int64_t i) {
        writeBytes(reinterpret_cast<std::byte*>(&i), sizeof(i));
        return *this;
    }

    Blob& Blob::operator<<(uint8_t i) {
        writeBytes(reinterpret_cast<std::byte*>(&i), sizeof(i));
        return *this;
    }

    Blob& Blob::operator<<(uint16_t i) {
        writeBytes(reinterpret_cast<std::byte*>(&i), sizeof(i));
        return *this;
    }

    Blob& Blob::operator<<(uint32_t i) {
        writeBytes(reinterpret_cast<std::byte*>(&i), sizeof(i));
        return *this;
    }

    Blob& Blob::operator<<(uint64_t i) {
        writeBytes(reinterpret_cast<std::byte*>(&i), sizeof(i));
        return *this;
    }

    Blob& Blob::operator<<(float i) {
        writeBytes(reinterpret_cast<std::byte*>(&i), sizeof(i));
        return *this;
    }

    Blob& Blob::operator<<(double i) {
        writeBytes(reinterpret_cast<std::byte*>(&i), sizeof(i));
        return *this;
    }

    Blob& Blob::operator<<(const Vec2& i) {
        writeBytes(reinterpret_cast<std::byte*>(const_cast<real*>(&i[0])), sizeof(i));
        return *this;
    }

    Blob& Blob::operator<<(const Vec3& i) {
        writeBytes(reinterpret_cast<std::byte*>(const_cast<real*>(&i[0])), sizeof(i));
        return *this;
    }

    Blob& Blob::operator<<(const Vec4& i) {
        writeBytes(reinterpret_cast<std::byte*>(const_cast<real*>(&i[0])), sizeof(i));
        return *this;
    }

    Blob& Blob::operator<<(const Quat& i) {
        writeBytes(reinterpret_cast<std::byte*>(const_cast<real*>(&i[0])), sizeof(i));
        return *this;
    }

    Blob& Blob::operator<<(const Color& i) {
        writeBytes(reinterpret_cast<std::byte*>(const_cast<real*>(&i[0])), sizeof(i));
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

    void Blob::writeBytes(std::byte* bytes, unsigned int count) {
        if (size() < count) {
            LOG_ERROR(logger, "buffer overflow");
            return;
        }

        std::copy(bytes, bytes + count, pointer());
    }
}  // namespace GLaDOS