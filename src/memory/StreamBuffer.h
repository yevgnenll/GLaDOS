#ifndef GAMEENGINE_STREAMBUFFER_H
#define GAMEENGINE_STREAMBUFFER_H

#include <cstdint>
#include <vector>

namespace GameEngine {
  class StreamBuffer {
  public:
    StreamBuffer() = default;
    ~StreamBuffer() = default;

    StreamBuffer& operator<<(int8_t i);
    StreamBuffer& operator<<(int16_t i);
    StreamBuffer& operator<<(int32_t i);
    StreamBuffer& operator<<(int64_t i);
    StreamBuffer& operator<<(uint8_t i);
    StreamBuffer& operator<<(uint16_t i);
    StreamBuffer& operator<<(uint32_t i);
    StreamBuffer& operator<<(uint64_t i);
    StreamBuffer& operator<<(float i);
    StreamBuffer& operator<<(double i);

    void* pointer();
    std::size_t size() const;

  private:
    void writeBytes(unsigned char* bytes, unsigned int count);

    std::vector<unsigned char> mData;
  };
}  // namespace GameEngine

#endif
