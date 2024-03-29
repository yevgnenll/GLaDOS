#ifndef GLADOS_MESHGENERATOR_H
#define GLADOS_MESHGENERATOR_H

#include "Utility.h"
#include "math/Rect.hpp"
#include "math/Vec3.h"

namespace GLaDOS {
    template <typename T>
    class Point;
    class Mesh;
    class MeshGenerator {
      public:
        MeshGenerator() = delete;
        ~MeshGenerator() = delete;

        static Mesh* generateRectangle(Rect<real> textureRect, const Size<uint32_t>& size);
        static Mesh* generatePlane(unsigned int dimensions = 50);
        static Mesh* generateCube();
        static Mesh* generateTexturedCube();
        static Mesh* generateIcoSphere(unsigned int subdivisions = 3);
        static Mesh* generateUVSphere();
        static Mesh* generateCapsule();
        static Mesh* generateCylinder();
        static Mesh* generateTorus(std::size_t mainSegments, std::size_t tubeSegments, real mainRadius, real tubeRadius);
        static Mesh* generateCone(real radius = 1.f, real height = 1.f);
        static Mesh* generateTeapot(unsigned int tesselation = 10);
        static Mesh* generateTorusKnot(std::size_t p, std::size_t q, std::size_t segments, std::size_t slices, real tubeRadius);
        static Mesh* generateFrustum(const Vec3& center, real fov, real maxRange, real minRange, real aspect);

        DISALLOW_COPY_AND_ASSIGN(MeshGenerator);
    };
}  // namespace GLaDOS

#endif  //GLADOS_MESHGENERATOR_H
