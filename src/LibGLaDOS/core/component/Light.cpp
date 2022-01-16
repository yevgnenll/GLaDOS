#include "Light.h"

namespace GLaDOS {
    Light::Light() : Component{"Light"} {

    }

    Light::~Light() {

    }

    void Light::update(real deltaTime) {
    }

    void Light::render() {
    }

    Component* Light::clone() {
        return nullptr;
    }
}