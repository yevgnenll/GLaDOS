#ifndef GLADOS_TRANSFORM_H
#define GLADOS_TRANSFORM_H

#include "core/Component.h"

namespace GLaDOS {
  class Transform : public Component {
  public:
    Transform() = default;
    ~Transform() override = default;

  private:
    virtual void update(real deltaTime);
    virtual void render();
  };
}  // namespace GLaDOS

#endif  //GLADOS_TRANSFORM_H
