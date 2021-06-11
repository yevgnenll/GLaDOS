#ifndef GLADOS_ABSTRACTFRAME_H
#define GLADOS_ABSTRACTFRAME_H

#include "AbstractView.h"

namespace GLaDOS::GUI {
  class ViewBuffer;
  class AbstractFrame : public AbstractView {
  public:
  private:
    FrameFlags mFrameFlags;
    ViewBuffer* mViewBuffer;
  };
}  // namespace GLaDOS::GUI

#endif  //GLADOS_ABSTRACTFRAME_H
