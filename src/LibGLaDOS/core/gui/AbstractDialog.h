#ifndef GLADOS_ABSTRACTDIALOG_H
#define GLADOS_ABSTRACTDIALOG_H

#include "AbstractRoundFrame.h"

namespace GLaDOS {
  class AbstractDialog : public AbstractRoundFrame {
  public:
    AbstractDialog();
    ~AbstractDialog() override;

  protected:
    virtual Response draw(AbstractWindow* context) = 0;
  };
}  // namespace GLaDOS

#endif  //GLADOS_ABSTRACTDIALOG_H
