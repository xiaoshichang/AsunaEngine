
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

    float depth = shadowMap.Sample(sampler, lightPosition.xy).r;
    if (lightPosition.z > depth)
    {
        return 0.5;
    }
    return 1.0f;
}