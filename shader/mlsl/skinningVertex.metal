#include <metal_stdlib>

using namespace metal;

constant int MAX_BONES = 96;
constant int MAX_BONE_INFLUENCE = 4;

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
  float3 _normal;
  float3 _tangent;
  float2 _texCoord0;
  float3 _fragPos;
} VertexOut;

typedef struct {
  float4x4 model;
  float4x4 modelViewProj;
  float4x4 transInvModelView;
  float4x4 boneTransform[MAX_BONES];
} VertexUniforms;

vertex VertexOut main0(VertexIn verts [[stage_in]], constant VertexUniforms &uniforms [[buffer(0)]]) {
    float4x4 skin = uniforms.boneTransform[verts._boneIndex.x] * verts._boneWeight.x;
    skin += uniforms.boneTransform[verts._boneIndex.y] * verts._boneWeight.y;
    skin += uniforms.boneTransform[verts._boneIndex.z] * verts._boneWeight.z;
    skin += uniforms.boneTransform[verts._boneIndex.w] * verts._boneWeight.w;

    float4 skinnedPosition = skin * float4(verts._position, 1.0f);
    VertexOut out;
    out._position = uniforms.modelViewProj * skinnedPosition;
    out._normal = float3(uniforms.transInvModelView * skin * float4(verts._normal, 0.f));
    out._tangent = float3(uniforms.transInvModelView * skin * float4(verts._tangent, 0.f));
    out._texCoord0 = verts._texCoord0;
    out._fragPos = float3(uniforms.model * skinnedPosition);

    return out;
}