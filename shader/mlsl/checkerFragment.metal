#include <metal_stdlib>

using namespace metal;

typedef struct {
  float4 _position [[position]];
  float2 _texCoord0;
} VertexOut;

typedef struct {
  float4 color0;
  float4 color1;
  float3 viewPos;
  bool isWireFrameMode;
} FragmentUniforms;

fragment float4 main0(VertexOut verts [[stage_in]], constant FragmentUniforms &uniforms [[buffer(0)]]) {
    if (uniforms.isWireFrameMode) {
        return float4(0.10588, 0.76862, 0.16078, 1.0);
    }
    float2 c = floor(verts._texCoord0) * 0.5;
    float checker = 2 * fract(c.x + c.y);
    return checker * uniforms.color0 + (1 - checker) * uniforms.color1;
}