#include <metal_stdlib>

using namespace metal;

typedef struct {
  float3 _position [[attribute(0)]];
  float2 _texCoord0 [[attribute(2)]];
} VertexIn;

typedef struct {
  float4 _position [[position]];
  float2 _texCoord0;
} VertexOut;

typedef struct {
  float4x4 model;
  float4x4 modelViewProj;
  float4x4 transInvModelView;
} VertexUniforms;

vertex VertexOut main0(VertexIn verts [[stage_in]], constant VertexUniforms &uniforms [[buffer(0)]]) {
    VertexOut out;
    out._position = uniforms.modelViewProj * float4(verts._position, 1);
    out._texCoord0 = verts._texCoord0;
    return out;
}