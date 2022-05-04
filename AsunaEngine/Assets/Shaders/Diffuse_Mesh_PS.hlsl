#include "VertexData.hlsl"
#include "ConstantBuffer.hlsl"

Texture2D tBaseMap : register(t0);
SamplerState tBaseMapSamplerState : register(s0);

float4 main(VertexOutput_P4N4T4 input) : SV_TARGET
{
    float4 baseColor = tBaseMap.Sample(tBaseMapSamplerState, input.texcoord.xy);

    float3 diffLight = max(dot(input.nw.xyz, -directionLight.direction.xyz), 0.0) * directionLight.color.xyz * directionLight.intensity.x;
    float3 envLight = float3(0.2, 0.2, 0.2);
    float3 lighting = min(diffLight + envLight, 1.0);
    return float4(baseColor.xyz * lighting, 1);
}

