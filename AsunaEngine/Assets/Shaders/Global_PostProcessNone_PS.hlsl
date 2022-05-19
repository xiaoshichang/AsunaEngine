#include "VertexData.hlsl"
#include "ConstantBuffer.hlsl"

Texture2D tBaseMap : register(t0);
SamplerState tBaseMapSamplerState : register(s0);

float4 main(VertexOutput_P4T4 input) : SV_TARGET
{
    float4 baseColor = tBaseMap.Sample(tBaseMapSamplerState, input.texcoord.xy);
    return float4(baseColor.r, baseColor.g, baseColor.b, 1);
}
