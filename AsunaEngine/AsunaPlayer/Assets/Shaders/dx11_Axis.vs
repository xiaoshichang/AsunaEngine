
struct a2v {
	float3 position : POSITION;
};

struct v2p {
	float4 position : SV_POSITION;
};

cbuffer ConstantBufferPerFrame : register(b0)
{
    matrix viewMatrix;
    matrix projectionMatrix;
    matrix vp;
};

cbuffer ConstantBufferPerObject : register(b1)
{
    matrix worldMatrix;
}

cbuffer ConstantBufferPerMaterial : register(b2)
{
    float4 mainColor;
    matrix modelMatrix;
}

v2p main(a2v input)
{
	v2p output;
	float4 HPos = float4(input.position, 1.0);
	output.position = HPos;
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
	return output;
}
