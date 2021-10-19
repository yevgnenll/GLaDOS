struct VertexOut {
	float4 _position : SV_POSITION;
    float4 _color : COLOR;
};

float4 main0(VertexOut pin) : SV_Target {
    return pin.Color;
}