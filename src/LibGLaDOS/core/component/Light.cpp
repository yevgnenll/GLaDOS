#include "Light.h"

namespace GLaDOS {
    Light::Light() : Component{"Light"} {

    }

    Light::~Light() {

    }

    void Light::fixedUpdate(real fixedDeltaTime) {
    }

    void Light::update(real deltaTime) {
    }

    void Light::render() {
    }

    Component* Light::clone() {
        return nullptr;
    }
}