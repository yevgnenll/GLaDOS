#ifndef GLADOS_ABSTRACTROUNDFRAME_H
#define GLADOS_ABSTRACTROUNDFRAME_H

#include "AbstractFrame.h"

namespace GLaDOS::GUI {
  class AbstractWindow;
  class AbstractRoundFrame : public AbstractFrame {
  public:

  protected:
    virtual bool preDraw(AbstractWindow* context);
  };
}

#endif  //GLADOS_ABSTRACTROUNDFRAME_H
