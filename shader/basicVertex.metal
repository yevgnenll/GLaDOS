#include <metal_stdlib>

using namespace metal;

typedef struct {
  float3 _position [[attribute(0)]];
  float4 _color [[attribute(1)]];
} VertexIn;

typedef struct {
  float4 _position [[position]];
  float4 _color;
} VertexOut;

typedef struct {
  float4x4 model;
  float4x4 view;
  float4x4 projection;
} VertexUniforms;

vertex VertexOut main0(VertexIn verts [[stage_in]], constant VertexUniforms &uniforms [[buffer(0)]]) {
    VertexOut out;
    out._position = uniforms.projection * uniforms.view * uniforms.model * float4(verts._position, 1);
    out._color = verts._color;
    return out;
}