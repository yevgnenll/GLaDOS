struct VertexIn {
	float3 _position : POSITION;
    float4 _color : COLOR;
};

struct VertexOut {
	float4 _position : SV_POSITION;
    float4 _color : COLOR;
};

cbuffer VertexUniforms : register(b0) {
    float4x4 model;
    float4x4 view;
    float4x4 projection;
};

VertexOut main0(VertexIn verts) {
    VertexOut out;
    out._position = mul(mul(mul(float4(verts._position, 1.0f), model), view), projection);
    out._color = verts._color;
    return out;
}