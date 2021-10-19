#include <metal_stdlib>

using namespace metal;

typedef struct {
  float4 _position [[position]];
  float4 _texCoords;
} VertexOut;

typedef struct {
  bool isWireFrameMode;
} FragmentUniforms;

fragment float4 main0(
    VertexOut verts [[stage_in]],
    texturecube<float> texCube [[texture(0)]],
    sampler cubeSampler [[sampler(0)]],
    constant FragmentUniforms &uniforms [[buffer(0)]]) {
  if (uniforms.isWireFrameMode) {
    return float4(0.10588, 0.76862, 0.16078, 1.0);
  }
  float3 texCoords = float3(verts._texCoords.x, verts._texCoords.y, -verts._texCoords.z);
  return texCube.sample(cubeSampler, texCoords);
}