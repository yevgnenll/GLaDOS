#include <metal_stdlib>

using namespace metal;

typedef struct {
  float3 _position [[attribute(0)]];
  float3 _normal [[attribute(1)]];
  float2 _texCoord0 [[attribute(2)]];
} VertexIn;

typedef struct {
  float4 _position [[position]];
  float2 _texCoord0;
  float4 _color;
} VertexOut;

typedef struct {
  float4x4 model;
  float4x4 modelViewProj;
  float4x4 transInvModelView;
  float2 scrollSpeed;
  float4 color;
} VertexUniforms;

vertex VertexOut main0(VertexIn verts [[stage_in]], constant VertexUniforms &uniforms [[buffer(0)]]) {
    VertexOut out;
    out._position = uniforms.modelViewProj * float4(verts._position, 1);
    out._texCoord0 = verts._texCoord0;
    out._texCoord0.x += uniforms.scrollSpeed.x;
    out._texCoord0.y += uniforms.scrollSpeed.y;
    out._color = uniforms.color;
    return out;
}