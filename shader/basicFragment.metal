#include <metal_stdlib>

using namespace metal;

fragment float4 main(
  float4 in [[stage_in]]
) {
  return float4(1, 1, 1, 1);
}