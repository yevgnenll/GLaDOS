#include "Message.h"
#include <cstring>

namespace GLaDOS {
  Message::Message(MessageType type) : mType(type) {}

  Message::Message(MessageType type, void* data, std::size_t size) : mType(type) {
    mData.resize(size);
    std::memcpy(mData.pointer(), data, size);
  }

  Message::Message(const Message& rhs) {
    mType = rhs.mType;
    mData = rhs.mData;
  }

  Message& Message::operator=(const Message& rhs) {
    mType = rhs.mType;
    mData = rhs.mData;
    return *this;
  }

  MessageType Message::type() const { return mType; }

  void* Message::data() { return mData.pointer(); }
}  // namespace GLaDOS