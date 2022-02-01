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
  int32_t _boneIndex [[attribute(5)]];
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
  float4x4 view;
  float4x4 projection;
  float4x4 invModelView;
  float4x4 boneTransform[MAX_BONES];
} VertexUniforms;

vertex VertexOut main0(VertexIn verts [[stage_in]], constant VertexUniforms &uniforms [[buffer(0)]]) {
    VertexOut out;
    /*
    float4 weightedPosition = float4(0, 0, 0, 0);
    float3 weightedNormal = float3(0, 0, 0);
    float3 weightedTangent = float3(0, 0, 0);

    for (int i = 0; i < MAX_BONE_INFLUENCE; i++) {
        int8_t boneIndex = ((verts._boneIndex >> (8 * i)) & 0xFF);
        if (boneIndex <= -1 || boneIndex > MAX_BONES) {
            continue;
        }
        float4 localPosition = uniforms.boneTransform[boneIndex] * float4(verts._position, 1.0f);
        weightedPosition += localPosition * verts._boneWeight[i];
        float3 localNormal = float3(uniforms.boneTransform[boneIndex] * float4(verts._normal, 0));
        weightedNormal += localNormal * verts._boneWeight[i];
        float3 localTangent = float3(uniforms.boneTransform[boneIndex] * float4(verts._tangent, 0));
        weightedTangent += localTangent * verts._boneWeight[i];
    }
    */

    float4 weightedPosition = uniforms.boneTransform[0] * float4(verts._position, 1.0f);
    float3 weightedNormal = verts._normal;
    float3 weightedTangent = verts._tangent;

    out._position = uniforms.projection * uniforms.view * uniforms.model * weightedPosition;
    out._normal = float3(transpose(uniforms.invModelView) * float4(weightedNormal, 0));
    out._tangent = float3(transpose(uniforms.invModelView) * float4(weightedTangent, 0));
    out._texCoord0 = verts._texCoord0;
    out._fragPos = float3(uniforms.model * weightedPosition);

    return out;
}