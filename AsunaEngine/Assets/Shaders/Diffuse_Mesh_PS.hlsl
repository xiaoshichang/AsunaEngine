#include "VertexData.hlsl"
#include "ConstantBuffer.hlsl"

Texture2D tBaseMap : register(t0);
SamplerState tBaseMapSamplerState : register(s0);

Texture2D tShadowMap : register(t1);
SamplerState tShadowMapSamplerState : register(s1);

float4 main(VertexOutput_P4N4T4L4 input) : SV_TARGET
{
    float4 baseColor = tBaseMap.Sample(tBaseMapSamplerState, input.texcoord.xy);
    float shadow = CalculationShadow(tShadowMap, tShadowMapSamplerState, input.lightPosition);
    float3 diffuseLight = max(dot(input.nw.xyz, -directionLight.direction.xyz), 0.0f) * directionLight.color.xyz * directionLight.intensity.x;
    float3 envLight = GetEnvLight();
    float3 lighting = (envLight + diffuseLight * shadow);
    lighting = min(lighting, 1) * baseColor.xyz;
    return float4(lighting, 1);
}

