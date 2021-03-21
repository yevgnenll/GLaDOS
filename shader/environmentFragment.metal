#include <metal_stdlib>

using namespace metal;

typedef struct {
  float4 _position [[position]];
  float4 _texCoords;
} VertexOut;

fragment float4 main0(VertexOut verts [[stage_in]], texturecube<float> texCube [[texture(0)]], sampler cubeSampler [[sampler(0)]]) {
    float3 texCoords = float3(verts._texCoords.x, verts._texCoords.y, -verts._texCoords.z);
    return texCube.sample(cubeSampler, texCoords);
}