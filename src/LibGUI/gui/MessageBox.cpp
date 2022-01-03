#include "MessageBox.h"

namespace GLaDOS::GUI {
    MessageBox::MessageBox(const std::string &title, const std::string &description) {
    }

    MessageBox::~MessageBox() {
    }

    void MessageBox::setTitleFont(const Font &font) {
    }

    void MessageBox::setTextFont(const Font &font) {
    }

    void MessageBox::sizeUpdate(const AbstractView *source, const AbstractView *target, int width, int height) {
        AbstractView::sizeUpdate(source, target, width, height);
    }

    bool MessageBox::preDraw(AbstractWindow *context) {
        return AbstractRoundFrame::preDraw(context);
    }

    Response MessageBox::draw(AbstractWindow *context) {
        return Response::Finish;
    }

    void MessageBox::onClose() {
    }
}