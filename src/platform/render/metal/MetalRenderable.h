#ifndef GLADOS_METALRENDERABLE_H
#define GLADOS_METALRENDERABLE_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_MACOS

#include "platform/render/Renderable.h"

namespace GLaDOS {
  class MetalRenderable : public Renderable {
  public:
    explicit MetalRenderable(RenderableId _id);
    ~MetalRenderable() override;
    void release() override;

  private:
  };
}  // namespace GLaDOS

#endif

#endif
