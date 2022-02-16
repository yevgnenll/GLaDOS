#include <metal_stdlib>

using namespace metal;

typedef struct {
  float4 _position [[position]];
  float3 _normal;
  float3 _tangent;
  float2 _texCoord0;
  float3 _fragPos;
} VertexOut;

typedef struct {
  float3 viewPos;
  float4 albedo;
  float4 specular;
  bool isWireFrameMode;
} FragmentUniforms;

constant float4 lightColor = float4(1.0);
constant float3 lightPos = float3(0.0, 10.0, 0.0);
constant float ambientStrength = 0.7;
constant float shininess = 16;
constant float pi = 3.14159265;

fragment float4 main0(VertexOut verts [[stage_in]], constant FragmentUniforms &uniforms [[buffer(0)]]) {
    if (uniforms.isWireFrameMode) {
        return float4(0.10588, 0.76862, 0.16078, 1.0);
    }

    // ambient color
	float4 ambient = ambientStrength * lightColor;

	// diffuse color
	float3 norm = normalize(verts._normal);
	float3 lightDir = normalize(lightPos - verts._fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	float4 diffuse = diff * lightColor;

	// specular color
    float3 viewDir = normalize(uniforms.viewPos - verts._fragPos);
    float3 halfwayDir = normalize(lightDir + viewDir);
    float EnergyConservation = ( 8.0 + shininess ) / ( 8.0 * pi );
    float spec = EnergyConservation * pow(max(dot(norm, halfwayDir), 0.0), shininess);
    float4 specular = spec * lightColor * uniforms.specular;

	float4 result = (ambient + diffuse + specular) * uniforms.albedo;
	return result;
}