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

vec4 _56;

void main()
{
    vec4 _65 = texture(SPIRV_Cross_CombinedtBaseMaptBaseMapSamplerState, in_var_TEXCOORD0.xy);
    float _132;
    do
    {
        vec3 _73 = in_var_TEXCOORD1.xyz / vec3(in_var_TEXCOORD1.w);
        float _74 = _73.x;
        float _78 = _73.y;
        float _83 = _73.z;
        if ((((((_74 < (-1.0)) || (_74 > 1.0)) || (_78 < (-1.0))) || (_78 > 1.0)) || (_83 < 0.0)) || (_83 > 1.0))
        {
            _132 = 0.0;
            break;
        }
        vec4 _92 = _56;
        _92.x = (_74 * 0.5) + 0.5;
        vec4 _95 = _92;
        _95.y = (_78 * 0.5) + 0.5;
        uvec2 _96 = uvec2(textureSize(SPIRV_Cross_CombinedtShadowMapSPIRV_Cross_DummySampler, int(0u)));
        vec2 _102 = vec2(1.0) / vec2(float(_96.x), float(_96.y));
        float _104;
        int _107;
        _104 = 0.0;
        _107 = -1;
        float _105;
        for (; _107 <= 1; _104 = _105, _107++)
        {
            _105 = _104;
            for (int _115 = -1; _115 <= 1; )
            {
                _105 += (((_83 - 0.00999999977648258209228515625) > texture(SPIRV_Cross_CombinedtShadowMaptShadowMapSamplerState, _95.xy + (vec2(float(_107), float(_115)) * _102)).x) ? 0.0 : 1.0);
                _115++;
                continue;
            }
        }
        _132 = _104 * 0.111111111938953399658203125;
        break;
    } while(false);
    out_var_SV_TARGET = vec4(min(vec3(0.20000000298023223876953125) + (((ConstantBufferPerFrame.directionLight.color.xyz * max(dot(in_var_NORMAL.xyz, -ConstantBufferPerFrame.directionLight.direction.xyz), 0.0)) * ConstantBufferPerFrame.directionLight.intensity.x) * _132), vec3(1.0)) * _65.xyz, 1.0);
}

