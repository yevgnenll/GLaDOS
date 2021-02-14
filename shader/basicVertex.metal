#include <metal_stdlib>

using namespace metal;

typedef struct {
  float4 _position [[attribute(0)]];
  float4 _color [[attribute(1)]];
} VertexIn;

typedef struct {
  float4 _position [[position]];
  float4 _color;
} VertexOut;

vertex VertexOut main0(VertexIn verts [[stage_in]]) {
    VertexOut out;
    out._position = verts._position;
    out._color = verts._color;
    return out;
}