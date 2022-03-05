#include "AbstractRoundFrame.h"

namespace GLaDOS {
    AbstractRoundFrame::AbstractRoundFrame() {
    }

    AbstractRoundFrame::~AbstractRoundFrame() {
    }

    bool AbstractRoundFrame::preDraw(AbstractWindow *context) {
        return false;
    }
}