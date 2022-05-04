#include "VertexData.hlsl"
#include "ConstantBuffer.hlsl"

Texture2D tBaseMap : register(t0);
SamplerState tBaseMapSamplerState : register(s0);

float4 main(VertexOutput_P4N4T4 input) : SV_TARGET
{
    float4 baseColor = tBaseMap.Sample(tBaseMapSamplerState, input.texcoord.xy);
    float diffLight = max(dot(input.nw.xyz, -directionLight.direction.xyz), 0.0);
    float envLight = 0.4;
    float lighting = (diffLight + envLight);
    return float4(baseColor.xyz * lighting, 1);
}

