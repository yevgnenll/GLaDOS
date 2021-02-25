#include "StreamBuffer.h"

namespace GLaDOS {
  StreamBuffer& StreamBuffer::operator<<(int8_t i) {
    writeBytes(reinterpret_cast<std::byte*>(&i), sizeof(i));
    return *this;
  }

  StreamBuffer& StreamBuffer::operator<<(int16_t i) {
    writeBytes(reinterpret_cast<std::byte*>(&i), sizeof(i));
    return *this;
  }

  StreamBuffer& StreamBuffer::operator<<(int32_t i) {
    writeBytes(reinterpret_cast<std::byte*>(&i), sizeof(i));
    return *this;
  }

  StreamBuffer& StreamBuffer::operator<<(int64_t i) {
    writeBytes(reinterpret_cast<std::byte*>(&i), sizeof(i));
    return *this;
  }

  StreamBuffer& StreamBuffer::operator<<(uint8_t i) {
    writeBytes(reinterpret_cast<std::byte*>(&i), sizeof(i));
    return *this;
  }

  StreamBuffer& StreamBuffer::operator<<(uint16_t i) {
    writeBytes(reinterpret_cast<std::byte*>(&i), sizeof(i));
    return *this;
  }

  StreamBuffer& StreamBuffer::operator<<(uint32_t i) {
    writeBytes(reinterpret_cast<std::byte*>(&i), sizeof(i));
    return *this;
  }

  StreamBuffer& StreamBuffer::operator<<(uint64_t i) {
    writeBytes(reinterpret_cast<std::byte*>(&i), sizeof(i));
    return *this;
  }

  StreamBuffer& StreamBuffer::operator<<(float i) {
    writeBytes(reinterpret_cast<std::byte*>(&i), sizeof(i));
    return *this;
  }

  StreamBuffer& StreamBuffer::operator<<(double i) {
    writeBytes(reinterpret_cast<std::byte*>(&i), sizeof(i));
    return *this;
  }

  void* StreamBuffer::offsetOf(std::size_t offset) {
    return static_cast<std::byte*>(pointer()) + offset;
  }

  void* StreamBuffer::pointer() {
    return &mData[0];
  }

  std::size_t StreamBuffer::size() const {
    return mData.size();
  }

  void StreamBuffer::resize(std::size_t n) {
    mData.resize(n);
  }

  void StreamBuffer::clear() {
    mData.clear();
  }

  bool StreamBuffer::isEmpty() const {
    return mData.empty();
  }

  void StreamBuffer::writeBytes(std::byte* bytes, unsigned int count) {
    if (size() < count) {
      LOG_ERROR("buffer overflow");
      return;
    }

    for (std::size_t i = 0; i < count; i++) {
      mData[i] = bytes[i];
    }
  }
}  // namespace GLaDOS