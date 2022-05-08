#include "VertexData.hlsl"
#include "ConstantBuffer.hlsl"

VertexOutput_P4T4 main(VertexInput_P3T3 input)
{
	VertexOutput_P4T4 output;
	output.position = float4(input.position, 1.0);
    output.texcoord = float4(input.texcoord, 1.0);
	return output;
}