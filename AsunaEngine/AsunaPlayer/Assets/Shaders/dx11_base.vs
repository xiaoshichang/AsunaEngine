
struct a2v {
	float3 position : POSITION;
	float3 normal : NORMAL;
	float3 texcoord : TEXCOORD;
};

struct v2p {
	float4 position : SV_POSITION;
	float4 nw : NORMAL;
	float4 texcoord : TEXCOORD;
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
    matrix modelMatrix;
}

v2p main(a2v input) 
{
	v2p output;
	float4 HPos = float4(input.position, 1.0);
	output.position = mul(HPos, modelMatrix);
	output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    float4 HNormal = float4(input.normal, 0.0);
    output.nw = normalize(mul(HNormal, worldMatrix));
    output.texcoord = float4(input.texcoord.xyz, 1);
	return output;
}
