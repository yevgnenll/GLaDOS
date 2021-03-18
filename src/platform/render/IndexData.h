#ifndef GLADOS_INDEXDATA_H
#define GLADOS_INDEXDATA_H

#include "ByteBuffer.h"

namespace GLaDOS {
  class IndexData : public ByteBuffer {
  public:
    IndexData(std::size_t stride, std::size_t count);
    ~IndexData() override = default;
  };
}  // namespace GLaDOS

#endif  //GLADOS_INDEXDATA_H
