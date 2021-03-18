#include <metal_stdlib>

using namespace metal;

typedef struct {
  float4 _position [[position]];
  float2 _texCoord0;
} VertexOut;

fragment float4 main0(VertexOut verts [[stage_in]], texture2d<float> tex2D [[texture(0)]], sampler sampler2D [[sampler(0)]]) {
    float4 sampledColor = tex2D.sample(sampler2D, verts._texCoord0);
    return sampledColor;
}