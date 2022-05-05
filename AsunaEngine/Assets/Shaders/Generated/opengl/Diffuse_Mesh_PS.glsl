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
} ConstantBufferPerFrame;

uniform sampler2D SPIRV_Cross_CombinedtBaseMaptBaseMapSamplerState;

layout(location = 0) in vec4 in_var_NORMAL;
layout(location = 1) in vec4 in_var_TEXCOORD;
layout(location = 0) out vec4 out_var_SV_TARGET;

void main()
{
    out_var_SV_TARGET = vec4(texture(SPIRV_Cross_CombinedtBaseMaptBaseMapSamplerState, in_var_TEXCOORD.xy).xyz * min(((ConstantBufferPerFrame.directionLight.color.xyz * max(dot(in_var_NORMAL.xyz, -ConstantBufferPerFrame.directionLight.direction.xyz), 0.0)) * ConstantBufferPerFrame.directionLight.intensity.x) + vec3(0.20000000298023223876953125), vec3(1.0)), 1.0);
}

