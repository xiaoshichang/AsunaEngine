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
layout(binding = 1) uniform sampler2D SPIRV_Cross_CombinedtShadowMaptShadowMapSamplerState;

layout(location = 0) in vec4 in_var_NORMAL;
layout(location = 1) in vec4 in_var_TEXCOORD0;
layout(location = 2) in vec4 in_var_TEXCOORD1;
layout(location = 0) out vec4 out_var_SV_TARGET;

vec4 _51;

void main()
{
    vec4 _60 = texture(SPIRV_Cross_CombinedtBaseMaptBaseMapSamplerState, in_var_TEXCOORD0.xy);
    float _98;
    do
    {
        vec3 _68 = in_var_TEXCOORD1.xyz / vec3(in_var_TEXCOORD1.w);
        float _69 = _68.x;
        float _73 = _68.y;
        float _78 = _68.z;
        if ((((((_69 < (-1.0)) || (_69 > 1.0)) || (_73 < (-1.0))) || (_73 > 1.0)) || (_78 < 0.0)) || (_78 > 1.0))
        {
            _98 = 0.0;
            break;
        }
        vec4 _87 = _51;
        _87.x = (_69 * 0.5) + 0.5;
        vec4 _90 = _87;
        _90.y = (_73 * 0.5) + 0.5;
        if (_78 > texture(SPIRV_Cross_CombinedtShadowMaptShadowMapSamplerState, _90.xy).x)
        {
            _98 = 0.5;
            break;
        }
        _98 = 1.0;
        break;
    } while(false);
    out_var_SV_TARGET = vec4((_60.xyz * min(((ConstantBufferPerFrame.directionLight.color.xyz * max(dot(in_var_NORMAL.xyz, -ConstantBufferPerFrame.directionLight.direction.xyz), 0.0)) * ConstantBufferPerFrame.directionLight.intensity.x) + vec3(0.20000000298023223876953125), vec3(1.0))) * _98, 1.0);
}

