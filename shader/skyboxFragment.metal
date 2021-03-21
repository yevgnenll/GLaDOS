#include <metal_stdlib>

using namespace metal;

typedef struct {
  float4 _position [[position]];
  float3 _normal;
  float3 _texCoord0;
} VertexOut;

fragment float4 main0(VertexOut verts [[stage_in]], texturecube<float> texCube [[texture(0)]], sampler cubeSampler [[sampler(0)]]) {
    float3 texCoord = float3(verts._texCoord0.x, verts._texCoord0.y, -verts._texCoord0.z);
    return texCube.sample(cubeSampler, texCoord);
}