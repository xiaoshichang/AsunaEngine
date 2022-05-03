
struct a2v {
	float3 position : POSITION;
};

struct v2p {
	float4 position : SV_POSITION;
};

struct DirectionLight
{
    float4 direction;
    float4 color;
    float4 intensity;
};

struct SpotLight
{
    float4 position;
    float4 color;
    float4 intensity;
};

cbuffer ConstantBufferPerFrame : register(b0)
{
    matrix viewMatrix;
    matrix projectionMatrix;
    matrix vp;
    DirectionLight directionLight;
    SpotLight spotLights[4];
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
	output.position = mul(HPos, modelMatrix);
	output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
	return output;
}
