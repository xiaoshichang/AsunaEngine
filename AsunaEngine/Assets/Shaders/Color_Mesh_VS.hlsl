#include "VertexData.hlsl"
#include "ConstantBuffer.hlsl"

VertexOutput_P4N4 main(VertexInput_P3N3 input)
{
	VertexOutput_P4N4 output;
	float4 HPos = float4(input.position, 1.0);
	output.position = mul(HPos, modelMatrix);
	output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    float4 HNormal = float4(input.normal, 0.0);
    output.nw = normalize(mul(HNormal, modelMatrix));
    output.nw = normalize(mul(output.nw, worldMatrix));

	return output;
}
