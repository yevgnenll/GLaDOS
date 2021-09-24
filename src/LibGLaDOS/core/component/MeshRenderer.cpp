#include "MeshRenderer.h"

#include "platform/Platform.h"
#include "platform/render/Renderable.h"
#include "platform/render/Renderer.h"

namespace GLaDOS {
    Logger* MeshRenderer::logger = LoggerRegistry::getInstance().makeAndGetLogger("MeshRenderer");
    MeshRenderer::MeshRenderer() : Component{"MeshRenderer"} {
    }

    MeshRenderer::MeshRenderer(Mesh* mesh, Material* material) : Component{"MeshRenderer"} {
        Renderable* renderable = Platform::getRenderer().createRenderable(mesh, material);
        if (renderable == nullptr) {
            LOG_ERROR(logger, "MeshRenderer initialize failed!");
        }
        mRenderable = renderable;
    }

    MeshRenderer::~MeshRenderer() {
        DELETE_T(mRenderable, Renderable);
    }

    void MeshRenderer::setRenderable(Renderable* renderable) {
        mRenderable = renderable;
    }

    void MeshRenderer::update(real deltaTime) {
    }

    void MeshRenderer::render() {
        Platform::getRenderer().render(mRenderable);
    }
}  // namespace GLaDOS