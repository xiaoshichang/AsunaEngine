#include "VertexData.hlsl"
#include "ConstantBuffer.hlsl"

Texture2D tShadowMap : register(t1);
SamplerState tShadowMapSamplerState : register(s1);

float4 main(VertexOutput_P4N4L4 input) : SV_TARGET
{
    float NDotL = max(dot(input.nw.xyz, -directionLight.direction.xyz), 0.0f);
    float shadow = CalculationShadow(tShadowMap, tShadowMapSamplerState, input.lightPosition, NDotL);
    float3 diffuse = NDotL * directionLight.color.xyz * directionLight.intensity.x;
    return float4(diffuse.xyz * mainColor.rgb * shadow, 1.0f);
}

