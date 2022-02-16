#include "BoneRenderer.h"

namespace GLaDOS {
    BoneRenderer::BoneRenderer() {
    }

    BoneRenderer::~BoneRenderer() {
    }

    void BoneRenderer::render() {
        MeshRenderer::render();
    }

    void BoneRenderer::update(real deltaTime) {
        MeshRenderer::update(deltaTime);
    }

    Component* BoneRenderer::clone() {
        return MeshRenderer::clone();
    }
}