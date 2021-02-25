#ifndef GLADOS_CAMERA_H
#define GLADOS_CAMERA_H

#include "core/Component.h"

namespace GLaDOS {
  class Camera : public Component {
  public:
    Camera() = default;
    ~Camera() override = default;

  private:
    virtual void update(real deltaTime);
    virtual void render();
  };
}  // namespace GLaDOS

#endif  //GLADOS_CAMERA_H
