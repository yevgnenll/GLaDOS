#include <metal_stdlib>

using namespace metal;

typedef struct {
  float4 _position [[position]];
  float3 _normal;
  float3 _tangent;
  float2 _texCoord0;
  float3 _fragPos;
} VertexOut;

typedef struct {
  float3 viewPos;
  bool isWireFrameMode;
} FragmentUniforms;

fragment float4 main0(VertexOut verts [[stage_in]], constant FragmentUniforms &uniforms [[buffer(0)]]) {
    if (uniforms.isWireFrameMode) {
        return float4(0.10588, 0.76862, 0.16078, 1.0);
    }

    return float4(normalize(verts._normal) * .5 + .5, 1);
}