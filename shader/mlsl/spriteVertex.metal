#include <metal_stdlib>

using namespace metal;

typedef struct {
  float3 _position [[attribute(0)]];
  float2 _texCoord0 [[attribute(1)]];
} VertexIn;

typedef struct {
  float4 _position [[position]];
  float2 _texCoord0;
} VertexOut;

typedef struct {
  float4x4 model;
  float4x4 view;
  float4x4 projection;
  bool flipX;
  bool flipY;
  float2 flipOffset;
  float2 anchorPoint;
  uint2 size;
} VertexUniforms;

vertex VertexOut main0(VertexIn verts [[stage_in]], constant VertexUniforms &uniforms [[buffer(0)]]) {
    VertexOut out;
    float2 anchorPointOffset = uniforms.anchorPoint;
    if (uniforms.flipX) {
        verts._texCoord0.x = uniforms.flipOffset.x - verts._texCoord0.x;
        anchorPointOffset.x = uniforms.size.x - uniforms.anchorPoint.x;
    }
    if (uniforms.flipY) {
        verts._texCoord0.y = uniforms.flipOffset.y - verts._texCoord0.y;
        anchorPointOffset.y = uniforms.size.y - uniforms.anchorPoint.y;
    }
    out._position = uniforms.projection * uniforms.view * uniforms.model *
                    float4(verts._position.x - anchorPointOffset.x,
                           verts._position.y - anchorPointOffset.y, verts._position.z, 1);
    out._texCoord0 = verts._texCoord0;
    return out;
}