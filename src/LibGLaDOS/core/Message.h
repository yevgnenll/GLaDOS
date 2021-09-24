#ifndef GLADOS_MESSAGE_H
#define GLADOS_MESSAGE_H

#include "memory/StreamBuffer.h"
#include "utils/Enumeration.h"

namespace GLaDOS {
    class Message {
      public:
        explicit Message(MessageType type);
        Message(MessageType type, void* data, std::size_t size);

        Message(const Message& rhs);
        Message& operator=(const Message& rhs);

        MessageType type() const;
        void* data();

      private:
        MessageType mType{MessageType::Undefined};
        StreamBuffer mData;
    };
}  // namespace GLaDOS

#endif  //GLADOS_MESSAGE_H
