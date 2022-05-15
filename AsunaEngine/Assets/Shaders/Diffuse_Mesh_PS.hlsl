#include "VertexData.hlsl"
#include "ConstantBuffer.hlsl"

Texture2D tBaseMap : register(t0);
SamplerState tBaseMapSamplerState : register(s0);

Texture2D tShadowMap : register(t1);
SamplerState tShadowMapSamplerState : register(s1);

float4 main(VertexOutput_P4N4T4L4 input) : SV_TARGET
{
    float NDotL = max(dot(input.nw.xyz, -directionLight.direction.xyz), 0.0f);

    float4 baseColor = tBaseMap.Sample(tBaseMapSamplerState, input.texcoord.xy);
    float shadow = CalculationShadow(tShadowMap, tShadowMapSamplerState, input.lightPosition, NDotL);
    float3 diffLight = NDotL * directionLight.color.xyz * directionLight.intensity.x;
    float3 envLight = float3(0.2, 0.2, 0.2);
    float3 lighting = min(diffLight + envLight, 1.0);
    return float4(baseColor.xyz * lighting * shadow, 1);
}

