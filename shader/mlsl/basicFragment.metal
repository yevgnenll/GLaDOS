#include <metal_stdlib>

using namespace metal;

typedef struct {
  float4 _position [[position]];
  float4 _color;
} VertexOut;

typedef struct {
  float brightness;
  bool isWireFrameMode;
} FragmentUniforms;

fragment float4 main0(VertexOut verts [[stage_in]], constant FragmentUniforms &uniforms [[buffer(0)]]) {
    if (uniforms.isWireFrameMode) {
        return float4(0.10588, 0.76862, 0.16078, 1.0);
    }
    return float4(uniforms.brightness * verts._color.rgb, verts._color.a);
}