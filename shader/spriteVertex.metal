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
} VertexUniforms;

vertex VertexOut main0(VertexIn verts [[stage_in]], constant VertexUniforms &uniforms [[buffer(0)]]) {
    VertexOut out;
    float2 resultTexCoord = verts._texCoord0;
    if (uniforms.flipX) {
        resultTexCoord = float2(1.0 - resultTexCoord.x, resultTexCoord.y);
    }
    if (uniforms.flipY) {
        resultTexCoord = float2(resultTexCoord.x, 1.0 - resultTexCoord.y);
    }
    out._position = uniforms.projection * uniforms.view * uniforms.model * float4(verts._position, 1);
    out._texCoord0 = resultTexCoord;
    return out;
}