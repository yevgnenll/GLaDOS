#include "Blob.h"

#include "math/Color.h"
#include "math/Quat.h"
#include "math/Vec2.h"
#include "math/Vec3.h"
#include "math/Vec4.h"

namespace GLaDOS {
    Logger* Blob::logger = LoggerRegistry::getInstance().makeAndGetLogger("Blob");
    Blob::Blob(std::size_t size, void* data) {
        resize(size);
        std::memcpy(pointer(), data, size);
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

    void Blob::uploadData(Blob& buffer) {
        throwIfOverflow(buffer.size());
        std::memcpy(pointer(), buffer.pointer(), buffer.size());
    }

    void Blob::uploadData(const Vector<std::byte>& data) {
        throwIfOverflow(data.size());
        std::memcpy(pointer(), data.data(), data.size());
    }

    void Blob::uploadData(const std::byte* data, const std::size_t size) {
        throwIfOverflow(size);
        std::memcpy(pointer(), data, size);
    }

    void* Blob::offsetOf(std::size_t offset) {
        return CAST(std::byte*, pointer()) + offset;
    }

    void* Blob::pointer() {
        return &mData[0];
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

        for (std::size_t i = 0; i < count; i++) {
            mData[i] = bytes[i];
        }
    }
}  // namespace GLaDOS