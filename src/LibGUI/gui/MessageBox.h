#ifndef GLADOS_MESSAGEBOX_H
#define GLADOS_MESSAGEBOX_H

#include "AbstractDialog.h"

namespace GLaDOS::GUI {
  class Font;
  class String;
  class Label;
  class CloseButton;
  class MessageBox : public AbstractDialog {
  public:
    MessageBox(const String& title, const String& description);
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
}  // namespace GLaDOS::GUI

#endif  //GLADOS_MESSAGEBOX_H
