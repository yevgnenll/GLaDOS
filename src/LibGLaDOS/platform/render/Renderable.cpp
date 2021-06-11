#include "Renderable.h"

#include "Material.h"
#include "Mesh.h"
#include "utils/Utility.h"

namespace GLaDOS {
  Logger* Renderable::logger = LoggerRegistry::getInstance().makeAndGetLogger("Renderable");
  Renderable::Renderable() {
    LOG_TRACE(logger, "Renderable instance created: {0}", mInstanceId);
  }

  Renderable::~Renderable() {
    DELETE_T(mMesh, Mesh);
    DELETE_T(mMaterial, Material);
  }
}  // namespace GLaDOS