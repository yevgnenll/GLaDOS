#ifndef GLADOS_INDEXDATA_H
#define GLADOS_INDEXDATA_H

#include "ByteContainer.h"

namespace GLaDOS {
  class IndexData : public ByteContainer {
  public:
    IndexData() = default;
    ~IndexData() override = default;
  };
}  // namespace GLaDOS

#endif  //GLADOS_INDEXDATA_H
