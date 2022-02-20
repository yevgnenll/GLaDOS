#include "LightSource.h"

namespace GLaDOS {
    LightSource::LightSource() : Component{"Light"} {

    }

    LightSource::~LightSource() {

    }

    void LightSource::fixedUpdate(real fixedDeltaTime) {
    }

    void LightSource::update(real deltaTime) {
    }

    void LightSource::render() {
    }

    Component* LightSource::clone() {
        return nullptr;
    }
}