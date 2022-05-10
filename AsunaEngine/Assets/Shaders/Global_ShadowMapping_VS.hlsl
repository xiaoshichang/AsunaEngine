#include "VertexData.hlsl"
#include "ConstantBuffer.hlsl"

VertexOutput_P4 main(VertexInput_P3 input)
{
    VertexOutput_P4 output;
    float4 HPos = float4(input.position, 1.0);
    output.position = mul(HPos, modelMatrix);
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, lightViewProj);
    return output;
}