#include <catch.hpp>

#include "platform/render/VertexData.h"

using namespace GLaDOS;

// clang-format off
float quad[] = {
    0.5, -0.5, 0.0, 1.0,     1.0, 0.0, 0.0, 1.0,
    -0.5, -0.5, 0.0, 1.0,     0.0, 1.0, 0.0, 1.0,
    -0.5,  0.5, 0.0, 1.0,     0.0, 0.0, 1.0, 1.0,

    0.5,  0.5, 0.0, 1.0,     1.0, 1.0, 0.0, 1.0,
    0.5, -0.5, 0.0, 1.0,     1.0, 0.0, 0.0, 1.0,
    -0.5,  0.5, 0.0, 1.0,     0.0, 0.0, 1.0, 1.0,
};
// clang-format on
std::size_t vertexCount = 6;
std::size_t stride = 8;

void testVertexData(VertexData& vd) {
  for (std::size_t i = 0; i < vertexCount * stride; i += stride) {
    std::size_t index = i / stride;
    Vec4 pos{quad[i], quad[i + 1], quad[i + 2], quad[i + 3]};
    REQUIRE(pos == vd.getPosition(index));
    Color color{quad[i + 4], quad[i + 5], quad[i + 6], quad[i + 7]};
    REQUIRE(color == vd.getColor(index));
  }
}

TEST_CASE("VertexData unit tests", "[VertexData]") {
  SECTION("Simple vertexData serialize & deserialize test") {
    VertexData vd{VertexFormatBuilder().withPosition().withColor(), vertexCount};
    REQUIRE(vd.count() == vertexCount);
    REQUIRE(vd.stride() == stride * sizeof(float));

    for (std::size_t i = 0; i < vertexCount * stride; i += stride) {
      std::size_t index = i / stride;
      vd.setPosition(index, Vec4(quad[i], quad[i + 1], quad[i + 2], quad[i + 3]));
      vd.setColor(index, Color(quad[i + 4], quad[i + 5], quad[i + 6], quad[i + 7]));
    }
    testVertexData(vd);
  }

  SECTION("Bulk vertex data upload with Vector of bytes test") {
    VertexData vd{VertexFormatBuilder().withPosition().withColor(), vertexCount};
    Vector<std::byte> data;
    for (float i : quad) {
      for (std::size_t j = 0; j < sizeof(float); j++) {
        auto* bytes = reinterpret_cast<std::byte*>(&i);
        data.push_back(bytes[j]);
      }
    }
    vd.uploadData(data);
    testVertexData(vd);
  }

  SECTION("Bulk vertex data upload with raw pointer of bytes test") {
    VertexData vd{VertexFormatBuilder().withPosition().withColor(), vertexCount};
    vd.uploadData(reinterpret_cast<std::byte*>(quad));
    testVertexData(vd);
  }
}
