#include <catch2/catch_all.hpp>

#include "platform/render/VertexData.h"

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

void testVertexData(VertexData& vd) {
  for (std::size_t i = 0; i < vertexCount * stride; i += stride) {
    std::size_t index = i / stride;
    Vec3 pos{quad[i], quad[i + 1], quad[i + 2]};
    REQUIRE(pos == vd.getPosition(index));
    Color color{quad[i + 3], quad[i + 4], quad[i + 5], quad[i + 6]};
    REQUIRE(color == vd.getColor(index));
  }
}

TEST_CASE("VertexData unit tests", "[VertexData]") {
  SECTION("Simple vertexData serialize & deserialize test") {
    VertexData vd{VertexFormatDescriptor().position().color(), vertexCount, true};
    REQUIRE(vd.count() == vertexCount);
    REQUIRE(vd.stride() == stride * sizeof(float));
    REQUIRE(vd.size() == vd.count() * vd.stride());

    for (std::size_t i = 0; i < vertexCount * stride; i += stride) {
      std::size_t index = i / stride;
      vd.setPosition(index, Vec3{quad[i], quad[i + 1], quad[i + 2]});
      vd.setColor(index, Color{quad[i + 3], quad[i + 4], quad[i + 5], quad[i + 6]});
    }
    testVertexData(vd);
  }

  SECTION("Bulk vertex data upload with Vector of bytes test") {
    VertexData vd{VertexFormatDescriptor().position().color(), vertexCount};
    Vector<std::byte> vertex;
    for (float i : quad) {
      for (std::size_t j = 0; j < sizeof(float); j++) {
        auto* bytes = reinterpret_cast<std::byte*>(&i);
        vertex.push_back(bytes[j]);
      }
    }
    vd.uploadDataNoCopy(vertex.data());
    testVertexData(vd);
  }

  SECTION("Bulk vertex data upload with raw pointer of bytes test") {
    VertexData vd{VertexFormatDescriptor().position().color(), vertexCount};
    vd.uploadDataNoCopy(quad);
    testVertexData(vd);
  }
}
