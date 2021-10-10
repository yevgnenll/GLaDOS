#ifndef GLADOS_INDEXDATA_H
#define GLADOS_INDEXDATA_H

#include "UploadBuffer.h"

namespace GLaDOS {
    class IndexData : public UploadBuffer {
      public:
        IndexData(std::size_t stride, std::size_t count, bool allocate = false);
        ~IndexData() override = default;
    };
}  // namespace GLaDOS

#endif  //GLADOS_INDEXDATA_H
