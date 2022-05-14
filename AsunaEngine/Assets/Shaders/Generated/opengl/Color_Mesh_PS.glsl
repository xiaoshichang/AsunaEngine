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

layout(binding = 1) uniform sampler2D SPIRV_Cross_CombinedtShadowMaptShadowMapSamplerState;

layout(location = 0) in vec4 in_var_NORMAL;
layout(location = 1) in vec4 in_var_TEXCOORD0;
layout(location = 0) out vec4 out_var_SV_TARGET;

vec4 _48;

void main()
{
    float _103;
    do
    {
        vec3 _73 = in_var_TEXCOORD0.xyz / vec3(in_var_TEXCOORD0.w);
        float _74 = _73.x;
        float _78 = _73.y;
        float _83 = _73.z;
        if ((((((_74 < (-1.0)) || (_74 > 1.0)) || (_78 < (-1.0))) || (_78 > 1.0)) || (_83 < 0.0)) || (_83 > 1.0))
        {
            _103 = 0.0;
            break;
        }
        vec4 _92 = _48;
        _92.x = (_74 * 0.5) + 0.5;
        vec4 _95 = _92;
        _95.y = (_78 * 0.5) + 0.5;
        if (_83 > texture(SPIRV_Cross_CombinedtShadowMaptShadowMapSamplerState, _95.xy).x)
        {
            _103 = 0.5;
            break;
        }
        _103 = 1.0;
        break;
    } while(false);
    out_var_SV_TARGET = vec4((((ConstantBufferPerFrame.directionLight.color.xyz * max(dot(in_var_NORMAL.xyz, -ConstantBufferPerFrame.directionLight.direction.xyz), 0.0)) * ConstantBufferPerFrame.directionLight.intensity.x) * ConstantBufferPerMaterial.mainColor.xyz) * _103, 1.0);
}

