#include <metal_stdlib>

using namespace metal;

typedef struct {
  float3 _position [[attribute(0)]];
  float3 _normal [[attribute(1)]];
} VertexIn;

typedef struct {
  float4 _position [[position]];
  float3 _normal;
  float3 _texCoord0;
} VertexOut;

typedef struct {
  float4x4 viewProjection;
} VertexUniforms;

vertex VertexOut main0(VertexIn verts [[stage_in]], constant VertexUniforms &uniforms [[buffer(0)]]) {
    VertexOut out;
    float4 position = uniforms.viewProjection * float4(verts._position, 1);
    out._position = position.xyww;
    out._normal = verts._normal;
    out._texCoord0 = verts._position;
    return out;
}