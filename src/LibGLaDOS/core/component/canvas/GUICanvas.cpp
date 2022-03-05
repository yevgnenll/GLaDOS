#include "GUICanvas.h"
#include "core/gui/AbstractFrame.h"

namespace GLaDOS {
    GUICanvas::GUICanvas() : Component("GUICanvas") {
    }

    GUICanvas::~GUICanvas() {
    }

    void GUICanvas::addFrame(AbstractFrame* frame) {

    }

    void GUICanvas::fixedUpdate(real fixedDeltaTime) {
    }

    void GUICanvas::update(real deltaTime) {
    }

    void GUICanvas::render() {
    }

    Component* GUICanvas::clone() {
        return nullptr;
    }
}