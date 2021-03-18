#include "IndexData.h"

namespace GLaDOS {
  IndexData::IndexData(std::size_t stride, std::size_t count) {
    mStride = stride;
    mStreamBuffer.resize(count * mStride);
  }
}  // namespace GLaDOS