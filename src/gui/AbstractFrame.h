#ifndef GLADOS_ABSTRACTFRAME_H
#define GLADOS_ABSTRACTFRAME_H

#include "utils/Enumeration.h"

namespace GLaDOS {
  class ViewBuffer;
  class AbstractFrame {
  public:
  private:
    FrameFlags mFrameFlags;
    ViewBuffer* mViewBuffer;
  };
}  // namespace GLaDOS

#endif  //GLADOS_ABSTRACTFRAME_H
