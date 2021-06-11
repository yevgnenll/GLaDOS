#include "SpriteRenderer.h"

#include "platform/Platform.h"
#include "platform/render/IndexData.h"
#include "platform/render/Material.h"
#include "platform/render/Mesh.h"
#include "platform/render/Renderable.h"
#include "platform/render/Renderer.h"
#include "platform/render/ShaderProgram.h"
#include "platform/render/VertexData.h"
#include "resource/Sprite.h"
#include "utils/MeshGenerator.h"

namespace GLaDOS {
  Logger* SpriteRenderer::logger = LoggerRegistry::getInstance().makeAndGetLogger("SpriteRenderer");
  SpriteRenderer::SpriteRenderer() {
    mName = "SpriteRenderer";
  }

  SpriteRenderer::SpriteRenderer(Sprite* sprite) : mSprite{sprite} {
    mName = "SpriteRenderer";
    setSprite(sprite);
  }

  SpriteRenderer::~SpriteRenderer() {
    DELETE_T(mSprite, Sprite);
  }

  void SpriteRenderer::setSprite(Sprite* sprite) {
    mSprite = sprite;

    Mesh* mesh = MeshGenerator::generateRectangle(mSprite->getTextureCoords());
    if (mesh == nullptr) {
      LOG_ERROR(logger, "SpriteRenderer initialize failed!");
      return;
    }
    ShaderProgram* shaderProgram = Platform::getRenderer().createShaderProgram("spriteVertex.metal", "spriteFragment.metal", mesh->getVertexData());
    if (shaderProgram == nullptr) {
      LOG_ERROR(logger, "SpriteRenderer initialize failed!");
      return;
    }

    Material* material = NEW_T(Material);
    material->setShaderProgram(shaderProgram);

    Renderable* renderable = Platform::getRenderer().createRenderable(mesh, material);
    if (renderable == nullptr) {
      LOG_ERROR(logger, "CubemapRenderer initialize failed!");
      return;
    }
    mRenderable = renderable;
  }

  void SpriteRenderer::setColor(const Color& color) {
    mColor = color;
  }

  void SpriteRenderer::setFlipX(bool flipX) {
    mFlipX = flipX;
  }

  void SpriteRenderer::setFlipY(bool flipY) {
    mFlipY = flipY;
  }

  Sprite* SpriteRenderer::getSprite() const {
    return mSprite;
  }

  Color SpriteRenderer::getColor() const {
    return mColor;
  }

  bool SpriteRenderer::getFlipX() const {
    return mFlipX;
  }

  bool SpriteRenderer::getFlipY() const {
    return mFlipY;
  }

  void SpriteRenderer::update(real deltaTime) {
    ShaderProgram* shaderProgram = mRenderable->getMaterial()->getShaderProgram();
    shaderProgram->setUniform("flipX", mFlipX);
    shaderProgram->setUniform("flipY", mFlipY);
    shaderProgram->setUniform("color", mColor);
  }

  void SpriteRenderer::render() {
    Platform::getRenderer().render(mRenderable);
  }
}  // namespace GLaDOS