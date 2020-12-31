#ifndef GAMEENGINE_RENDERER_H
#define GAMEENGINE_RENDERER_H

#include <vector>

namespace GameEngine {
  class Renderable;
  class Renderer {
  public:
    Renderer();
    virtual ~Renderer();

  private:
    std::vector<Renderable*> mRenderable;
  };
}  // namespace GameEngine

#endif
