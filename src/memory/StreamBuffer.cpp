#include "StreamBuffer.h"

namespace GameEngine {
  StreamBuffer& StreamBuffer::operator<<(int8_t i) {
    writeBytes(reinterpret_cast<unsigned char*>(&i), sizeof(i));
    return *this;
  }

  StreamBuffer& StreamBuffer::operator<<(int16_t i) {
    writeBytes(reinterpret_cast<unsigned char*>(&i), sizeof(i));
    return *this;
  }

  StreamBuffer& StreamBuffer::operator<<(int32_t i) {
    writeBytes(reinterpret_cast<unsigned char*>(&i), sizeof(i));
    return *this;
  }

  StreamBuffer& StreamBuffer::operator<<(int64_t i) {
    writeBytes(reinterpret_cast<unsigned char*>(&i), sizeof(i));
    return *this;
  }

  StreamBuffer& StreamBuffer::operator<<(uint8_t i) {
    writeBytes(reinterpret_cast<unsigned char*>(&i), sizeof(i));
    return *this;
  }

  StreamBuffer& StreamBuffer::operator<<(uint16_t i) {
    writeBytes(reinterpret_cast<unsigned char*>(&i), sizeof(i));
    return *this;
  }

  StreamBuffer& StreamBuffer::operator<<(uint32_t i) {
    writeBytes(reinterpret_cast<unsigned char*>(&i), sizeof(i));
    return *this;
  }

  StreamBuffer& StreamBuffer::operator<<(uint64_t i) {
    writeBytes(reinterpret_cast<unsigned char*>(&i), sizeof(i));
    return *this;
  }

  StreamBuffer& StreamBuffer::operator<<(float i) {
    writeBytes(reinterpret_cast<unsigned char*>(&i), sizeof(i));
    return *this;
  }

  StreamBuffer& StreamBuffer::operator<<(double i) {
    writeBytes(reinterpret_cast<unsigned char*>(&i), sizeof(i));
    return *this;
  }

  void* StreamBuffer::pointer() {
    return &mData[0];
  }

  std::size_t StreamBuffer::size() const {
    return mData.size();
  }

  void StreamBuffer::writeBytes(unsigned char* bytes, unsigned int count) {
    for (unsigned int i = 0; i < count; i++) {
      mData.push_back(bytes[i]);
    }
  }
}  // namespace GameEngine