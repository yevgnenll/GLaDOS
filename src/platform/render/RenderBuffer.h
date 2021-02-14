#ifndef GLADOS_RENDERBUFFER_H
#define GLADOS_RENDERBUFFER_H

#include "FrameBuffer.h"

namespace GLaDOS {
  class RenderBuffer : public FrameBuffer {
  public:
    RenderBuffer() = default;
    ~RenderBuffer() override = default;

    void begin() override;
    void end() override;
  };
}  // namespace GLaDOS

#endif  //GLADOS_RENDERBUFFER_H
