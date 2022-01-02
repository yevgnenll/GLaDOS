#include <metal_stdlib>

using namespace metal;

typedef struct {
  float4 _position [[position]];
  float2 _texCoord0;
} VertexOut;

typedef struct {
  float4 color;
  float4 colorKey;
  bool useColorKey;
} FragmentUniforms;

fragment float4 main0(VertexOut verts [[stage_in]],
                      texture2d<float> tex2D [[texture(0)]],
                      sampler sampler2D [[sampler(0)]],
                      constant FragmentUniforms &uniforms [[buffer(0)]]) {
    float4 sampledColor = tex2D.sample(sampler2D, verts._texCoord0) * uniforms.color;

    if (sampledColor.a <= 0)
        discard_fragment();

    if (uniforms.useColorKey) {
        bool3 isDiscard = sampledColor.rgb == uniforms.colorKey.rgb;
        if(isDiscard.x == true && isDiscard.y == true && isDiscard.z == true)
            discard_fragment();
    }

    return sampledColor;
}