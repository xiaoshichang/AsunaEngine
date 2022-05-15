#version 420

struct DirectionLight
{
    vec4 direction;
    vec4 color;
    vec4 intensity;
};

struct SpotLight
{
    vec4 direction;
    vec4 position;
    vec4 color;
    vec4 intensity;
};

struct PointLight
{
    vec4 position;
    vec4 color;
    vec4 intensity;
};

layout(binding = 0, std140) uniform type_ConstantBufferPerFrame
{
    layout(row_major) mat4 viewMatrix;
    layout(row_major) mat4 projectionMatrix;
    layout(row_major) mat4 vp;
    DirectionLight directionLight;
    SpotLight spotLights[4];
    PointLight pointLights[4];
    layout(row_major) mat4 lightViewProj;
} ConstantBufferPerFrame;

layout(binding = 0) uniform sampler2D SPIRV_Cross_CombinedtBaseMaptBaseMapSamplerState;
layout(binding = 1) uniform sampler2D SPIRV_Cross_CombinedtShadowMapSPIRV_Cross_DummySampler;
layout(binding = 1) uniform sampler2D SPIRV_Cross_CombinedtShadowMaptShadowMapSamplerState;

layout(location = 0) in vec4 in_var_NORMAL;
layout(location = 1) in vec4 in_var_TEXCOORD0;
layout(location = 2) in vec4 in_var_TEXCOORD1;
layout(location = 0) out vec4 out_var_SV_TARGET;

vec4 _58;

void main()
{
    float _69 = max(dot(in_var_NORMAL.xyz, -ConstantBufferPerFrame.directionLight.direction.xyz), 0.0);
    vec4 _74 = texture(SPIRV_Cross_CombinedtBaseMaptBaseMapSamplerState, in_var_TEXCOORD0.xy);
    float _144;
    do
    {
        vec3 _82 = in_var_TEXCOORD1.xyz / vec3(in_var_TEXCOORD1.w);
        float _83 = _82.x;
        float _87 = _82.y;
        float _92 = _82.z;
        if ((((((_83 < (-1.0)) || (_83 > 1.0)) || (_87 < (-1.0))) || (_87 > 1.0)) || (_92 < 0.0)) || (_92 > 1.0))
        {
            _144 = 0.0;
            break;
        }
        vec4 _101 = _58;
        _101.x = (_83 * 0.5) + 0.5;
        vec4 _104 = _101;
        _104.y = (_87 * 0.5) + 0.5;
        float _107 = max(0.0500000007450580596923828125 * (1.0 - _69), 0.004999999888241291046142578125);
        uvec2 _108 = uvec2(textureSize(SPIRV_Cross_CombinedtShadowMapSPIRV_Cross_DummySampler, int(0u)));
        vec2 _114 = vec2(1.0) / vec2(float(_108.x), float(_108.y));
        float _116;
        int _119;
        _116 = 0.0;
        _119 = -1;
        float _117;
        for (; _119 <= 1; _116 = _117, _119++)
        {
            _117 = _116;
            for (int _127 = -1; _127 <= 1; )
            {
                _117 += (((_92 - _107) > texture(SPIRV_Cross_CombinedtShadowMaptShadowMapSamplerState, _104.xy + (vec2(float(_119), float(_127)) * _114)).x) ? 0.199999988079071044921875 : 1.0);
                _127++;
                continue;
            }
        }
        _144 = _116 * 0.111111111938953399658203125;
        break;
    } while(false);
    out_var_SV_TARGET = vec4((_74.xyz * min(((ConstantBufferPerFrame.directionLight.color.xyz * _69) * ConstantBufferPerFrame.directionLight.intensity.x) + vec3(0.20000000298023223876953125), vec3(1.0))) * _144, 1.0);
}

