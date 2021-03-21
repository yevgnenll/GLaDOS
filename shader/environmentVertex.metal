#include <metal_stdlib>

using namespace metal;

typedef struct {
  float3 _position [[attribute(0)]];
  float3 _normal [[attribute(1)]];
} VertexIn;

typedef struct {
  float4 _position [[position]];
  float4 _texCoords;
} VertexOut;

typedef struct {
  float4x4 model;
  float4x4 modelViewProj;
  float4x4 normal;
  float3 cameraPos;
} VertexUniforms;

// some common indices of refraction
constant float kEtaAir = 1.000277;
//constant float kEtaWater = 1.333;
constant float kEtaGlass = 1.5;
constant float kEtaRatio = kEtaAir / kEtaGlass;

vertex VertexOut main0(VertexIn verts [[stage_in]], constant VertexUniforms &uniforms [[buffer(0)]]) {
    float4 modelPosition = float4(verts._position, 1);
    float4 modelNormal = float4(verts._normal, 0);

    float4 worldCameraPosition = float4(uniforms.cameraPos, 1);
    float4 worldPosition = uniforms.model * modelPosition;
    float4 worldNormal = normalize(uniforms.normal * modelNormal);
    float4 worldEyeDirection = normalize(worldPosition - worldCameraPosition);

    VertexOut out;
    out._position = uniforms.modelViewProj * modelPosition;
    out._texCoords = reflect(worldEyeDirection, worldNormal); // refract(worldEyeDirection, -worldNormal, kEtaRatio);

    return out;
}