#include "VertexData.hlsl"
#include "ConstantBuffer.hlsl"

Texture2D tShadowMap : register(t1);
SamplerState tShadowMapSamplerState : register(s1);

float4 main(VertexOutput_P4N4L4 input) : SV_TARGET
{
    float3 diffuse = max(dot(input.nw.xyz, -directionLight.direction.xyz), 0.0) * directionLight.color.xyz * directionLight.intensity.x;
    float shadow = CalculationShadow(tShadowMap, tShadowMapSamplerState, input.lightPosition);
    return float4(diffuse.xyz * mainColor.rgb * shadow, 1.0f);
}

