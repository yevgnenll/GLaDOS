#ifndef GAMEENGINE_RENDERER_H
#define GAMEENGINE_RENDERER_H

#include <vector>

namespace GameEngine {
  class Renderable;
  class Renderer {
  public:
    Renderer();
    virtual ~Renderer();

    virtual bool initialize() = 0;
    virtual void render(Renderable* renderable) const = 0;

  private:
    std::vector<Renderable*> mRenderable;
  };
}  // namespace GameEngine

#endif
