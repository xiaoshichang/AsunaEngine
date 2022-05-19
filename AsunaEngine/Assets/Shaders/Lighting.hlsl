
struct DirectionLight
{
    float4 direction;
    float4 color;
    float4 intensity;
};

struct SpotLight
{
    float4 direction;
    float4 position;
    float4 color;
    float4 intensity;
};

struct PointLight
{
    float4 position;
    float4 color;
    float4 intensity;
};


float3 GetEnvLight()
{
    return float3(0.2, 0.2, 0.2);
}

float PercentageCloserFilteringSoftShadow(Texture2D shadowMap, SamplerState sampler, float4 lightPosition)
{
    float bias = 0.01;
    float sw, sh, sl;
    shadowMap.GetDimensions(0, sw, sh, sl);
    float2 texelSize = 1.0f / float2(sw, sh);
    float shadow = 0.0;
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float depth = shadowMap.Sample(sampler, lightPosition.xy + float2(x, y) * texelSize).r;
            // lightPosition.z - bias > depth means current pixel in shadow
            shadow += lightPosition.z - bias > depth ? 0.0 : 1.0;
        }
    }
    return shadow /= 9.0;
}

float HardShadow(Texture2D shadowMap, SamplerState sampler, float4 lightPosition)
{
    float bias = 0.01;
    float depth = shadowMap.Sample(sampler, lightPosition.xy).r;
    return lightPosition.z - bias > depth ? 0.0 : 1.0;
}

float CalculationShadow(Texture2D shadowMap, SamplerState sampler, float4 lightPosition)
{
    lightPosition.xyz /= lightPosition.w;

    if (lightPosition.x < -1.0f || lightPosition.x > 1.0f ||
        lightPosition.y < -1.0f || lightPosition.y > 1.0f ||
        lightPosition.z < 0.0f || lightPosition.z > 1.0f)
        {
            return 0.0f;
        }

#ifdef _HLSL_
    lightPosition.x = lightPosition.x / 2 + 0.5;
    lightPosition.y = lightPosition.y / -2 + 0.5;
#endif
#ifdef _GLSL_
    lightPosition.x = lightPosition.x / 2 + 0.5;
    lightPosition.y = lightPosition.y / 2 + 0.5;
#endif

    // 0.0 -> in shadow
    // 1.0 -> in light
    // float shadow = HardShadow(shadowMap, sampler, lightPosition);
    float shadow = PercentageCloserFilteringSoftShadow(shadowMap, sampler, lightPosition);
    return shadow;
}
