#include "Renderable.h"

#include "Material.h"
#include "Mesh.h"
#include "utils/Utility.h"

namespace GLaDOS {
  Renderable::Renderable() {
    LOG_TRACE("default", "Renderable instance created: {0}", mInstanceId);
  }

  Renderable::~Renderable() {
    DELETE_T(mMesh, Mesh);
    DELETE_T(mMaterial, Material);
  }
}  // namespace GLaDOS