#ifndef GLADOS_ABSTRACTDIALOG_H
#define GLADOS_ABSTRACTDIALOG_H

#include "AbstractRoundFrame.h"

namespace GLaDOS::GUI {
  class AbstractDialog : public AbstractRoundFrame {
  public:
    AbstractDialog();
    virtual ~AbstractDialog();

  protected:
    virtual Response draw(AbstractWindow* context) = 0;
  };
}

#endif  //GLADOS_ABSTRACTDIALOG_H
