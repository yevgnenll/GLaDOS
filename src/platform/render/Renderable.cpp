#include "Renderable.h"
#include "utils/Utility.h"
#include "Mesh.h"
#include "Material.h"

namespace GLaDOS {
  Renderable::~Renderable() {
    DELETE_T(mMesh, Mesh);
    DELETE_T(mMaterial, Material);
  }
}