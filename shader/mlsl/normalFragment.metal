#include <metal_stdlib>

using namespace metal;

typedef struct {
  float4 _position [[position]];
  float3 _normal;
  float3 _fragPos;
} VertexOut;

typedef struct {
  float3 viewPos;
} FragmentUniforms;

constant float3 lightColor = float3(1.0, 1.0, 1.0);
constant float3 objectColor = float3(1.0, 1.0, 1.0);
constant float3 lightPos = float3(0.0, 10.0, 0.0);
constant float shininess = 16;
constant float pi = 3.14159265;

fragment float4 main0(VertexOut verts [[stage_in]], constant FragmentUniforms &uniforms [[buffer(0)]]) {
    // ambient color
	float ambientStrength = 0.1;
	float3 ambient = ambientStrength * lightColor;

	// diffuse color
	float3 norm = normalize(verts._normal);
	float3 lightDir = normalize(lightPos - verts._fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	float3 diffuse = diff * lightColor;

	// specular color
    float specularStrength = 0.5;
    float3 viewDir = normalize(uniforms.viewPos - verts._fragPos);
    float3 halfwayDir = normalize(lightDir + viewDir);
    float EnergyConservation = ( 8.0 + shininess ) / ( 8.0 * pi );
    float spec = EnergyConservation * pow(max(dot(norm, halfwayDir), 0.0), shininess);
    float3 specular = specularStrength * spec * lightColor;

	float4 result = float4((ambient + diffuse + specular) * objectColor, 1.0);
	return result;
}