#include <metal_stdlib>

using namespace metal;

typedef struct {
  float3 _position [[attribute(0)]];
  float3 _normal [[attribute(1)]];
  float3 _tangent [[attribute(2)]];
  float3 _biTangent [[attribute(3)]];
  float4 _boneWeight [[attribute(4)]];
  uint _boneIndex [[attribute(5)]];
  float2 _texCoord0 [[attribute(6)]];
} VertexIn;

typedef struct {
  float4 _position [[position]];
  float3 _normal;
  float2 _texCoord0;
  float3 _fragPos;
} VertexOut;

typedef struct {
  float4x4 model;
  float4x4 view;
  float4x4 projection;
  float4x4 invModelView;
} VertexUniforms;

vertex VertexOut main0(VertexIn verts [[stage_in]], constant VertexUniforms &uniforms [[buffer(0)]]) {
    VertexOut out;
    float4 position = float4(verts._position, 1);
    out._position = uniforms.projection * uniforms.view * uniforms.model * position;
    out._normal = float3(transpose(uniforms.invModelView) * float4(verts._normal, 0));
    out._texCoord0 = verts._texCoord0;
    out._fragPos = float3(uniforms.model * position);
    return out;
}