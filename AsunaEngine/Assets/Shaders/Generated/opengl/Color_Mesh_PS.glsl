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

layout(binding = 2, std140) uniform type_ConstantBufferPerMaterial
{
    vec4 mainColor;
    layout(row_major) mat4 modelMatrix;
} ConstantBufferPerMaterial;

layout(binding = 1) uniform sampler2D SPIRV_Cross_CombinedtShadowMapSPIRV_Cross_DummySampler;
layout(binding = 1) uniform sampler2D SPIRV_Cross_CombinedtShadowMaptShadowMapSamplerState;

layout(location = 0) in vec4 in_var_NORMAL;
layout(location = 1) in vec4 in_var_TEXCOORD0;
layout(location = 0) out vec4 out_var_SV_TARGET;

vec4 _56;

void main()
{
    float _126;
    do
    {
        vec3 _67 = in_var_TEXCOORD0.xyz / vec3(in_var_TEXCOORD0.w);
        float _68 = _67.x;
        float _72 = _67.y;
        float _77 = _67.z;
        if ((((((_68 < (-1.0)) || (_68 > 1.0)) || (_72 < (-1.0))) || (_72 > 1.0)) || (_77 < 0.0)) || (_77 > 1.0))
        {
            _126 = 0.0;
            break;
        }
        vec4 _86 = _56;
        _86.x = (_68 * 0.5) + 0.5;
        vec4 _89 = _86;
        _89.y = (_72 * 0.5) + 0.5;
        uvec2 _90 = uvec2(textureSize(SPIRV_Cross_CombinedtShadowMapSPIRV_Cross_DummySampler, int(0u)));
        vec2 _96 = vec2(1.0) / vec2(float(_90.x), float(_90.y));
        float _98;
        int _101;
        _98 = 0.0;
        _101 = -1;
        float _99;
        for (; _101 <= 1; _98 = _99, _101++)
        {
            _99 = _98;
            for (int _109 = -1; _109 <= 1; )
            {
                _99 += (((_77 - 0.00999999977648258209228515625) > texture(SPIRV_Cross_CombinedtShadowMaptShadowMapSamplerState, _89.xy + (vec2(float(_101), float(_109)) * _96)).x) ? 0.0 : 1.0);
                _109++;
                continue;
            }
        }
        _126 = _98 * 0.111111111938953399658203125;
        break;
    } while(false);
    out_var_SV_TARGET = vec4(min(vec3(0.20000000298023223876953125) + (((ConstantBufferPerFrame.directionLight.color.xyz * max(dot(in_var_NORMAL.xyz, -ConstantBufferPerFrame.directionLight.direction.xyz), 0.0)) * ConstantBufferPerFrame.directionLight.intensity.x) * _126), vec3(1.0)) * ConstantBufferPerMaterial.mainColor.xyz, 1.0);
}

