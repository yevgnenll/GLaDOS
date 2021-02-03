#include <metal_stdlib>

using namespace metal;

vertex float4 main(
  constant float4* in [[buffer(0)]],
  uint vid [[vertex_id]]
) {
  return in[vid];
}