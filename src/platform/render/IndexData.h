#ifndef GLADOS_INDEXDATA_H
#define GLADOS_INDEXDATA_H

#include "ByteBuffer.h"

namespace GLaDOS {
  class IndexData : public ByteBuffer {
  public:
    IndexData() = default;
    ~IndexData() override = default;
  };
}  // namespace GLaDOS

#endif  //GLADOS_INDEXDATA_H
