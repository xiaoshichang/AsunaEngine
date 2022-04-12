
struct a2v {
	float3 position : POSITION;
};

struct v2p {
	float4 position : SV_POSITION;
};

cbuffer ConstantBufferPerFrame
{
    matrix viewMatrix;
    matrix projectionMatrix;
    matrix vp;
};

cbuffer ConstantBufferPerObject
{
    matrix worldMatrix;
}

cbuffer ConstantBufferPerMaterial
{
    float4 mainColor;
}

v2p main(a2v input) 
{
	v2p output;
	float4 HPos = float4(input.position, 1.0);
	output.position = mul(HPos, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
	return output;
}
