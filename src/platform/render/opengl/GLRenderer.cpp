#include "GLRenderer.h"

#include "platform/OSTypes.h"

#ifdef PLATFORM_LINUX

namespace GLaDOS {
  GLRenderer::GLRenderer() {
  }

  GLRenderer::~GLRenderer() {
  }

  bool GLRenderer::initialize() {
  }

  void GLRenderer::render() const {
  }

  Buffer* GLRenderer::createVertexBuffer(BufferUsage usage, StreamBuffer& buffer) {
  }

  Buffer* GLRenderer::createIndexBuffer(BufferUsage usage, StreamBuffer& buffer) {
  }

  ShaderProgram* GLRenderer::createShaderProgram() {
  }
}  // namespace GLaDOS

#endif