#ifndef GLADOS_ABSTRACTROUNDFRAME_H
#define GLADOS_ABSTRACTROUNDFRAME_H

#include "AbstractFrame.h"

namespace GLaDOS {
  class AbstractWindow;
  class AbstractRoundFrame : public AbstractFrame {
  public:
    AbstractRoundFrame();
    ~AbstractRoundFrame() override;

  protected:
    virtual bool preDraw(AbstractWindow* context);
  };
}  // namespace GLaDOS

#endif  //GLADOS_ABSTRACTROUNDFRAME_H
