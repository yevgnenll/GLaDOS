#include <metal_stdlib>

using namespace metal;

typedef struct {
  float3 _position [[attribute(0)]];
  float3 _normal [[attribute(1)]];
  float3 _tangent [[attribute(2)]];
  float3 _biTangent [[attribute(3)]];
  float4 _boneWeight [[attribute(4)]];
  int4 _boneIndex [[attribute(5)]];
  float2 _texCoord0 [[attribute(6)]];
} VertexIn;

typedef struct {
  float4 _position [[position]];
} VertexOut;

typedef struct {
  float4x4 model;
  float4x4 modelViewProj;
  float4x4 transInvModelView;
} VertexUniforms;

vertex VertexOut main0(VertexIn verts [[stage_in]], constant VertexUniforms &uniforms [[buffer(0)]]) {
    VertexOut out;
    out._position = uniforms.modelViewProj * float4(verts._position, 1);
    return out;
}