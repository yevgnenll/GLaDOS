#include "MeshGenerator.h"

#include "math/UVec3.h"
#include "math/Vec3.h"
#include "math/Math.h"
#include "platform/Platform.h"
#include "platform/render/IndexBuffer.h"
#include "platform/render/Material.h"
#include "platform/render/Mesh.h"
#include "platform/render/Renderer.h"
#include "platform/render/VertexBuffer.h"
#include "platform/render/VertexFormat.h"

namespace GLaDOS {
    Mesh* MeshGenerator::generateRectangle(const Rect<real> textureRect, const Size<uint32_t>& size) {
        // Counter-closk wise
        // Vertex,Texture
        //	0----------3
        //	|          |
        //	1----------2
        real width = static_cast<real>(size.w);
        real height = static_cast<real>(size.h);

        Vector<real> vertices = {
            0.f, height, 0.f,
            textureRect.x, textureRect.h,

            0.f, 0.f, 0.f,
            textureRect.x, textureRect.y,

            width, 0.f, 0.f,
            textureRect.w, textureRect.y,

            width, height, 0.f,
            textureRect.w, textureRect.h
        };
        static Vector<uint16_t> indices = {
            0, 1, 2,
            0, 2, 3
        };

        VertexBuffer* vertexBuffer = NEW_T(VertexBuffer(VertexFormatDescriptor().position().texCoord0(), vertices.size()));
        vertexBuffer->copyBufferData(vertices.data());
        IndexBuffer* indexBuffer = NEW_T(IndexBuffer(sizeof(uint16_t), indices.size()));
        indexBuffer->copyBufferData(indices.data());
        return Platform::getRenderer().createMesh(vertexBuffer, indexBuffer);
    }

    Mesh* MeshGenerator::generatePlane(unsigned int dimensions) {
        std::size_t vertexCount = dimensions * dimensions * 7;
        std::size_t indexCount = (dimensions - 1) * (dimensions - 1) * 2 * 3;

        Vector<real> vertices;
        vertices.reserve(vertexCount);
        Vector<uint32_t> indices;
        indices.reserve(indexCount);

        real half = dimensions * 0.5f;
        for (unsigned int i = 0; i < dimensions; i++) {
            for (unsigned int j = 0; j < dimensions; j++) {
                vertices.emplace_back(real(j - half));
                vertices.emplace_back(0.f);
                vertices.emplace_back(real(i - half));

                vertices.emplace_back(0.f);
                vertices.emplace_back(1.f);
                vertices.emplace_back(0.f);
            }
        }

        for (unsigned int row = 0; row < dimensions - 1; row++) {
            for (unsigned int col = 0; col < dimensions - 1; col++) {
                indices.push_back(dimensions * row + col);
                indices.push_back(dimensions * row + col + dimensions);
                indices.push_back(dimensions * row + col + dimensions + 1);

                indices.push_back(dimensions * row + col);
                indices.push_back(dimensions * row + col + dimensions + 1);
                indices.push_back(dimensions * row + col + 1);
            }
        }

        VertexBuffer* vertexBuffer = NEW_T(VertexBuffer(VertexFormatDescriptor().position().normal(), vertexCount));
        vertexBuffer->copyBufferData(vertices.data());
        IndexBuffer* indexBuffer = NEW_T(IndexBuffer(sizeof(uint32_t), indexCount));
        indexBuffer->copyBufferData(indices.data());
        return Platform::getRenderer().createMesh(vertexBuffer, indexBuffer);
    }

    Mesh* MeshGenerator::generateCube() {
        static real vertices[] = {
            // +Y
            -0.5, 0.5, 0.5, 0.0, -1.0, 0.0,
            0.5, 0.5, 0.5, 0.0, -1.0, 0.0,
            0.5, 0.5, -0.5, 0.0, -1.0, 0.0,
            -0.5, 0.5, -0.5, 0.0, -1.0, 0.0,
            // -Y
            -0.5, -0.5, -0.5, 0.0, 1.0, 0.0,
            0.5, -0.5, -0.5, 0.0, 1.0, 0.0,
            0.5, -0.5, 0.5, 0.0, 1.0, 0.0,
            -0.5, -0.5, 0.5, 0.0, 1.0, 0.0,
            // +Z
            -0.5, -0.5, 0.5, 0.0, 0.0, -1.0,
            0.5, -0.5, 0.5, 0.0, 0.0, -1.0,
            0.5, 0.5, 0.5, 0.0, 0.0, -1.0,
            -0.5, 0.5, 0.5, 0.0, 0.0, -1.0,
            // -Z
            0.5, -0.5, -0.5, 0.0, 0.0, 1.0,
            -0.5, -0.5, -0.5, 0.0, 0.0, 1.0,
            -0.5, 0.5, -0.5, 0.0, 0.0, 1.0,
            0.5, 0.5, -0.5, 0.0, 0.0, 1.0,
            // -X
            -0.5, -0.5, -0.5, 1.0, 0.0, 0.0,
            -0.5, -0.5, 0.5, 1.0, 0.0, 0.0,
            -0.5, 0.5, 0.5, 1.0, 0.0, 0.0,
            -0.5, 0.5, -0.5, 1.0, 0.0, 0.0,
            // +X
            0.5, -0.5, 0.5, -1.0, 0.0, 0.0,
            0.5, -0.5, -0.5, -1.0, 0.0, 0.0,
            0.5, 0.5, -0.5, -1.0, 0.0, 0.0,
            0.5, 0.5, 0.5, -1.0, 0.0, 0.0};

        static uint16_t indices[] = {
            0, 3, 2, 2, 1, 0,
            4, 7, 6, 6, 5, 4,
            8, 11, 10, 10, 9, 8,
            12, 15, 14, 14, 13, 12,
            16, 19, 18, 18, 17, 16,
            20, 23, 22, 22, 21, 20};

        VertexBuffer* vertexBuffer = NEW_T(VertexBuffer(VertexFormatDescriptor().position().normal(), sizeof(vertices) / 6));
        vertexBuffer->copyBufferData(vertices);
        IndexBuffer* indexBuffer = NEW_T(IndexBuffer(sizeof(uint16_t), sizeof(indices) / sizeof(uint16_t)));
        indexBuffer->copyBufferData(indices);
        return Platform::getRenderer().createMesh(vertexBuffer, indexBuffer);
    }

    Mesh* MeshGenerator::generateTexturedCube() {
        // position, normal, texcoord
        static Vector<real> vertices = {
            // front
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.f, 1.f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.f, 1.f,
            0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.f, 0.f,
            0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.f, 0.f,
            -0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.f, 0.f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.f, 1.f,

            // back
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.f, 1.f,
             0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.f, 1.f,
             0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.f, 0.f,
             0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.f, 0.f,
            -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.f, 0.f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.f, 1.f,

            // left
            -0.5f,  0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.f, 1.f,
            -0.5f,  0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.f, 1.f,
            -0.5f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.f, 0.f,
            -0.5f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.f, 0.f,
            -0.5f, -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.f, 0.f,
            -0.5f,  0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.f, 1.f,

            // right
            0.5f, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.f, 0.f,
            0.5f,  0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.f, 1.f,
            0.5f,  0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 0.f, 1.f,
            0.5f,  0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 0.f, 1.f,
            0.5f, -0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 0.f, 0.f,
            0.5f, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.f, 0.f,

            // top
            0.5f, 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.f, 0.f,
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.f, 1.f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.f, 1.f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.f, 1.f,
            -0.5f, 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.f, 0.f,
            0.5f, 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.f, 0.f,

            // bottom
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.f, 1.f,
             0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.f, 1.f,
             0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.f, 0.f,
             0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.f, 0.f,
            -0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 0.f, 0.f,
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.f, 1.f
        };

        VertexBuffer* vertexBuffer = NEW_T(VertexBuffer(VertexFormatDescriptor().position().normal().texCoord0(), vertices.size() / 8));
        vertexBuffer->copyBufferData(vertices.data());
        return Platform::getRenderer().createMesh(vertexBuffer, nullptr);
    }

    Mesh* MeshGenerator::generateIcoSphere(unsigned int subdivisions) {
        real t = (1.f + Math::sqrt(5.f) / 2.f);
        static Vector<Vec3> vertices = {
            // position
            Vec3(-1.f, t, 0.f).makeNormalize(),
            Vec3(1.f, t, 0.f).makeNormalize(),
            Vec3(-1.f, -t, 0.f).makeNormalize(),
            Vec3(1.f, -t, 0.f).makeNormalize(),

            Vec3(0.f, -1.f, t).makeNormalize(),
            Vec3(0.f, 1.f, t).makeNormalize(),
            Vec3(0.f, -1.f, -t).makeNormalize(),
            Vec3(0.f, 1.f, -t).makeNormalize(),

            Vec3(t, 0.f, -1.f).makeNormalize(),
            Vec3(t, 0.f, 1.f).makeNormalize(),
            Vec3(-t, 0.f, -1.f).makeNormalize(),
            Vec3(-t, 0.f, 1.f).makeNormalize()};
        static Vector<uint32_t> indices = {
            0, 11, 5, 0, 5, 1, 0, 1, 7, 0, 7, 10, 0, 10, 11,
            1, 5, 9, 5, 11, 4, 11, 10, 2, 10, 7, 6, 7, 1, 8,
            3, 9, 4, 3, 4, 2, 3, 2, 6, 3, 6, 8, 3, 8, 9,
            4, 9, 5, 2, 4, 11, 6, 2, 10, 8, 6, 7, 9, 8, 1};

        auto subdivide = [](std::size_t p1, std::size_t p2) {
            Vec3 middle = (vertices[p1] + vertices[p2]) / 2.f;
            vertices.emplace_back(Vec3::normalize(middle));
            return vertices.size() - 1;
        };

        for (unsigned int i = 0; i < subdivisions; i++) {
            Vector<uint32_t> indices2;
            for (std::size_t j = 0; j < indices.size() / 3; j++) {
                uint32_t a = static_cast<uint32_t>(subdivide(indices[j * 3 + 0], indices[j * 3 + 1]));
                uint32_t b = static_cast<uint32_t>(subdivide(indices[j * 3 + 1], indices[j * 3 + 2]));
                uint32_t c = static_cast<uint32_t>(subdivide(indices[j * 3 + 2], indices[j * 3 + 0]));

                indices2.emplace_back(indices[j * 3 + 0]);
                indices2.emplace_back(a);
                indices2.emplace_back(c);

                indices2.emplace_back(indices[j * 3 + 1]);
                indices2.emplace_back(b);
                indices2.emplace_back(a);

                indices2.emplace_back(indices[j * 3 + 2]);
                indices2.emplace_back(c);
                indices2.emplace_back(b);

                indices2.emplace_back(a);
                indices2.emplace_back(b);
                indices2.emplace_back(c);
            }

            indices = indices2;
        }

        VertexBuffer* vertexBuffer = NEW_T(VertexBuffer(VertexFormatDescriptor().position().normal(), vertices.size() * 6));
        vertexBuffer->copyBufferData(vertices.data());
        IndexBuffer* indexBuffer = NEW_T(IndexBuffer(sizeof(uint32_t), indices.size()));
        indexBuffer->copyBufferData(indices.data());
        return Platform::getRenderer().createMesh(vertexBuffer, indexBuffer);
    }

    Mesh* MeshGenerator::generateUVSphere() {
        return nullptr;
    }

    Mesh* MeshGenerator::generateCapsule() {
        return nullptr;
    }

    Mesh* MeshGenerator::generateCylinder() {
        return nullptr;
    }

    Mesh* MeshGenerator::generateTorus(std::size_t mainSegments, std::size_t tubeSegments, real mainRadius, real tubeRadius) {
        std::size_t vertexCount = (mainSegments + 1) * (tubeSegments + 1) * 6;
        std::size_t indexCount = (mainSegments * 2 * (tubeSegments + 1)) + mainSegments - 1;

        Vector<real> vertices;
        vertices.reserve(vertexCount);
        Vector<real> indices;
        indices.reserve(indexCount);

        Rad mainSegmentAngleStep = Math::toRadians(Deg{static_cast<real>(Math::degrees / mainSegments)});
        Rad tubeSegmentAngleStep = Math::toRadians(Deg{static_cast<real>(Math::degrees / tubeSegments)});

        // vertex creation
        real currentMainSegmentAngle = 0.0;
        for (std::size_t i = 0; i <= mainSegments; i++) {
            // Calculate sine and cosine of main segment angle
            real sinMainSegment = Math::sin(currentMainSegmentAngle);
            real cosMainSegment = Math::cos(currentMainSegmentAngle);
            real currentTubeSegmentAngle = 0.0;

            for (std::size_t j = 0; j <= tubeSegments; j++) {
                // Calculate sine and cosine of tube segment angle
                real sinTubeSegment = Math::sin(currentTubeSegmentAngle);
                real cosTubeSegment = Math::cos(currentTubeSegmentAngle);

                // Calculate vertex p on the surface of torus
                Vec3 p{
                    (mainRadius + tubeRadius * cosTubeSegment) * cosMainSegment,
                    (mainRadius + tubeRadius * cosTubeSegment) * sinMainSegment,
                    tubeRadius * sinTubeSegment};

                vertices.emplace_back(p.x);
                vertices.emplace_back(p.y);
                vertices.emplace_back(p.z);

                Vec3 n = Vec3{
                    cosMainSegment * cosTubeSegment,
                    sinMainSegment * cosTubeSegment,
                    sinTubeSegment};
                vertices.emplace_back(n.x);
                vertices.emplace_back(n.y);
                vertices.emplace_back(n.z);

                // Update current tube angle
                currentTubeSegmentAngle += tubeSegmentAngleStep.get();
            }

            // Update main segment angle
            currentMainSegmentAngle += mainSegmentAngleStep.get();
        }

        // index creation
        uint32_t vi = 0;
        for (std::size_t i = 0; i < mainSegments; i++) {
            for (std::size_t j = 0; j <= tubeSegments; j++) {
                indices.emplace_back(vi);
                indices.emplace_back(vi + tubeSegments + 1);
                vi++;
            }

            // Don't restart primitive, if it's last segment, rendering ends here anyway
            if (i != mainSegments - 1) {
                indices.emplace_back(vertexCount);
            }
        }

        VertexBuffer* vertexBuffer = NEW_T(VertexBuffer(VertexFormatDescriptor().position().normal(), vertexCount));
        vertexBuffer->copyBufferData(vertices.data());
        IndexBuffer* indexBuffer = NEW_T(IndexBuffer(sizeof(uint32_t), indices.size()));
        indexBuffer->copyBufferData(indices.data());
        return Platform::getRenderer().createMesh(vertexBuffer, indexBuffer);
    }

    Mesh* MeshGenerator::generateCone(real radius, real height) {
        // TODO: NOT WORK -> Metal, D3DX10, Vulkan does not support TriangleFan Primitive type
        //    std::size_t vertexCount = 606;
        //
        //    Vector<real> vertices;
        //    vertices.reserve(vertexCount);
        //
        //    real phi = 0.f;
        //    int nPhi = 100;
        //    real dPhi = 2.f * Math::pi / (nPhi - 1);
        //
        //    // initial position
        //    vertices.emplace_back(0.f);
        //    vertices.emplace_back(height);
        //    vertices.emplace_back(0.f);
        //    // initial normal
        //    vertices.emplace_back(0.f);
        //    vertices.emplace_back(height);
        //    vertices.emplace_back(0.f);
        //
        //    for (int i = 0; i < nPhi; i++) {
        //      vertices.emplace_back(Math::cos(phi) * radius);
        //      vertices.emplace_back(-height);
        //      vertices.emplace_back(Math::sin(phi) * radius);
        //
        //      vertices.emplace_back(height * Math::cos(phi));
        //      vertices.emplace_back(radius);
        //      vertices.emplace_back(height * Math::sin(phi));
        //      phi += dPhi;
        //    }
        //
        //    VertexBuffer* vertexBuffer = NEW_T(VertexBuffer(VertexFormatBuilder().withPosition().withNormal(), vertexCount));
        //    vertexBuffer->copyBufferData(reinterpret_cast<std::byte*>(vertices.data()));
        //    Mesh* mesh = Platform::getRenderer().createMesh(vertexBuffer, nullptr);
        //    return mesh;
        return nullptr;
    }

    Mesh* MeshGenerator::generateTeapot(unsigned int tesselation) {
        return nullptr;
    }

    Mesh* MeshGenerator::generateTorusKnot(std::size_t p, std::size_t q, std::size_t segments, std::size_t slices, real tubeRadius) {
        // https://knotplot.com/zoo/
        std::size_t vertexCount = (segments + 1) * (slices + 1) * 6;
        std::size_t indexCount = (segments) * (slices + 1) * 6;

        Vector<real> vertices;
        vertices.reserve(vertexCount);
        Vector<uint32_t> indices;
        indices.reserve(indexCount);

        const real epsilon = 1e-4;
        const real dt = (2 * Math::pi) / (segments);
        const real du = (2 * Math::pi) / slices;

        for (std::size_t i = 0; i <= segments; ++i) {
            // calculate a point that lies on the curve
            real t0 = i * dt;
            real r0 = static_cast<real>((2 + Math::cos(q * t0)) * 0.5);
            Vec3 p0{r0 * Math::cos(p * t0), r0 * Math::sin(p * t0), -Math::sin(q * t0)};

            // approximate the Frenet frame { T, N, B } for the curve at the current point
            real t1 = t0 + epsilon;
            real r1 = static_cast<real>((2 + Math::cos(q * t1)) * 0.5);

            // p1 is p0 advanced infinitesimally along the curve
            Vec3 p1{r1 * Math::cos(p * t1), r1 * Math::sin(p * t1), -Math::sin(q * t1)};

            // compute approximate tangent as vector connecting p0 to p1
            Vec3 T = {p1.x - p0.x, p1.y - p0.y, p1.z - p0.z};

            // rough approximation of normal vector
            Vec3 N = {p1.x + p0.x, p1.y + p0.y, p1.z + p0.z};

            // compute binormal of curve
            Vec3 B = Vec3::cross(T, N);

            // refine normal vector by Graham-Schmidt
            N = Vec3::cross(B, T);

            N = Vec3::normalize(N);
            B = Vec3::normalize(B);

            // generate points in a circle perpendicular to the curve at the current point
            for (std::size_t j = 0; j <= slices; ++j) {
                real u = j * du;

                // compute position of circle point
                real x = tubeRadius * Math::cos(u);
                real y = tubeRadius * Math::sin(u);

                Vec3 p2{x * N.x + y * B.x, x * N.y + y * B.y, x * N.z + y * B.z};

                vertices.emplace_back(p0.x + p2.x);
                vertices.emplace_back(p0.y + p2.y);
                vertices.emplace_back(p0.z + p2.z);

                // compute normal of circle point
                Vec3 n2 = Vec3::normalize(p2);

                vertices.emplace_back(n2.x);
                vertices.emplace_back(n2.y);
                vertices.emplace_back(n2.z);
            }
        }

        // generate triplets of indices to create torus triangles
        for (std::size_t vi = 0; vi < (segments) * (slices + 1); ++vi) {
            indices.emplace_back(vi);
            indices.emplace_back(vi + slices + 1);
            indices.emplace_back(vi + slices);

            indices.emplace_back(vi);
            indices.emplace_back(vi + 1);
            indices.emplace_back(vi + slices + 1);
        }

        VertexBuffer* vertexBuffer = NEW_T(VertexBuffer(VertexFormatDescriptor().position().normal(), vertexCount));
        vertexBuffer->copyBufferData(vertices.data());
        IndexBuffer* indexBuffer = NEW_T(IndexBuffer(sizeof(uint32_t), indexCount));
        indexBuffer->copyBufferData(indices.data());
        return Platform::getRenderer().createMesh(vertexBuffer, indexBuffer);
    }

}  // namespace GLaDOS