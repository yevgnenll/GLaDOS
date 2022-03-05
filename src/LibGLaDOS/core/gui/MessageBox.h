#ifndef GLADOS_MESSAGEBOX_H
#define GLADOS_MESSAGEBOX_H

#include <string>

#include "AbstractDialog.h"

namespace GLaDOS {
  class Font;
  class Label;
  class CloseButton;
  class MessageBox : public AbstractDialog {
  public:
    MessageBox(const std::string& title, const std::string& description);
    ~MessageBox() override;

    void setTitleFont(const Font& font);
    void setTextFont(const Font& font);

  protected:
    void sizeUpdate(const AbstractView* source, const AbstractView* target, int width, int height) override;
    bool preDraw(AbstractWindow* context) override;
    Response draw(AbstractWindow* context) override;

  private:
    void onClose();

    Label* mTitle;
    Label* mText;
    CloseButton* mClose;
  };
}  // namespace GLaDOS

#endif  //GLADOS_MESSAGEBOX_H
