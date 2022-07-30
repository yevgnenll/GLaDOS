#include <catch2/catch_test_macros.hpp>

#include "platform/render/VertexBuffer.h"

using namespace GLaDOS;

// clang-format off
float quad[] = {
    0.5, -0.5, 0.0,     1.0, 0.0, 0.0, 1.0,
    -0.5, -0.5, 0.0,     0.0, 1.0, 0.0, 1.0,
    -0.5,  0.5, 0.0,     0.0, 0.0, 1.0, 1.0,

    0.5,  0.5, 0.0,     1.0, 1.0, 0.0, 1.0,
    0.5, -0.5, 0.0,     1.0, 0.0, 0.0, 1.0,
    -0.5,  0.5, 0.0,     0.0, 0.0, 1.0, 1.0,
};
// clang-format on
std::size_t vertexCount = 6;
std::size_t stride = 7;

void testVertexBuffer(VertexBuffer& vd) {
    for (std::size_t i = 0; i < vertexCount * stride; i += stride) {
        std::size_t index = i / stride;
        Vec3 pos{quad[i], quad[i + 1], quad[i + 2]};
        REQUIRE(pos == vd.getPosition(index));
        Color color{quad[i + 3], quad[i + 4], quad[i + 5], quad[i + 6]};
        REQUIRE(color == vd.getColor(index));
    }
}

TEST_CASE("VertexBuffer unit tests", "[VertexBuffer]") {
    SECTION("Simple vertexBuffer serialize & deserialize test") {
        VertexBuffer vd{VertexFormatDescriptor().position().color(), vertexCount};
        REQUIRE(vd.count() == vertexCount);
        REQUIRE(vd.stride() == stride * sizeof(float));
        REQUIRE(vd.size() == vd.count() * vd.stride());

        for (std::size_t i = 0; i < vertexCount * stride; i += stride) {
            std::size_t index = i / stride;
            vd.setPosition(index, Vec3{quad[i], quad[i + 1], quad[i + 2]});
            vd.setColor(index, Color{quad[i + 3], quad[i + 4], quad[i + 5], quad[i + 6]});
        }
        testVertexBuffer(vd);
    }

    SECTION("Bulk vertex buffer upload with Vector of bytes test") {
        VertexBuffer vd{VertexFormatDescriptor().position().color(), vertexCount};
        Vector<std::byte> vertex;
        for (float i : quad) {
            for (std::size_t j = 0; j < sizeof(float); j++) {
                auto* bytes = reinterpret_cast<std::byte*>(&i);
                vertex.push_back(bytes[j]);
            }
        }
        vd.copyBufferData(vertex.data());
        testVertexBuffer(vd);
    }

    SECTION("Bulk vertex buffer upload with raw pointer of bytes test") {
        VertexBuffer vd{VertexFormatDescriptor().position().color(), vertexCount};
        vd.copyBufferData(quad);
        testVertexBuffer(vd);
    }
}
