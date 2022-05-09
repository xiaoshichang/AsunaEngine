#include "VertexData.hlsl"
#include "ConstantBuffer.hlsl"

float4 main(VertexOutput_P4N4 input) : SV_TARGET
{
    float3 diffuse = max(dot(input.nw.xyz, -directionLight.direction.xyz), 0.0) * directionLight.color.xyz * directionLight.intensity.x;
    return float4(diffuse.xyz * mainColor.rgb, 1.0f);
}

