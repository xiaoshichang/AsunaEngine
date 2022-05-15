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

vec4 _55;

void main()
{
    float _65 = max(dot(in_var_NORMAL.xyz, -ConstantBufferPerFrame.directionLight.direction.xyz), 0.0);
    float _135;
    do
    {
        vec3 _73 = in_var_TEXCOORD0.xyz / vec3(in_var_TEXCOORD0.w);
        float _74 = _73.x;
        float _78 = _73.y;
        float _83 = _73.z;
        if ((((((_74 < (-1.0)) || (_74 > 1.0)) || (_78 < (-1.0))) || (_78 > 1.0)) || (_83 < 0.0)) || (_83 > 1.0))
        {
            _135 = 0.0;
            break;
        }
        vec4 _92 = _55;
        _92.x = (_74 * 0.5) + 0.5;
        vec4 _95 = _92;
        _95.y = (_78 * 0.5) + 0.5;
        float _98 = max(0.0500000007450580596923828125 * (1.0 - _65), 0.004999999888241291046142578125);
        uvec2 _99 = uvec2(textureSize(SPIRV_Cross_CombinedtShadowMapSPIRV_Cross_DummySampler, int(0u)));
        vec2 _105 = vec2(1.0) / vec2(float(_99.x), float(_99.y));
        float _107;
        int _110;
        _107 = 0.0;
        _110 = -1;
        float _108;
        for (; _110 <= 1; _107 = _108, _110++)
        {
            _108 = _107;
            for (int _118 = -1; _118 <= 1; )
            {
                _108 += (((_83 - _98) > texture(SPIRV_Cross_CombinedtShadowMaptShadowMapSamplerState, _95.xy + (vec2(float(_110), float(_118)) * _105)).x) ? 0.199999988079071044921875 : 1.0);
                _118++;
                continue;
            }
        }
        _135 = _107 * 0.111111111938953399658203125;
        break;
    } while(false);
    out_var_SV_TARGET = vec4((((ConstantBufferPerFrame.directionLight.color.xyz * _65) * ConstantBufferPerFrame.directionLight.intensity.x) * ConstantBufferPerMaterial.mainColor.xyz) * _135, 1.0);
}

