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

layout(binding = 2, std140) uniform type_ConstantBufferPerMaterial
{
    vec4 mainColor;
    layout(row_major) mat4 modelMatrix;
} ConstantBufferPerMaterial;

layout(location = 0) in vec4 in_var_NORMAL;
layout(location = 0) out vec4 out_var_SV_TARGET;

void main()
{
    out_var_SV_TARGET = vec4(((ConstantBufferPerFrame.directionLight.color.xyz * max(dot(in_var_NORMAL.xyz, -ConstantBufferPerFrame.directionLight.direction.xyz), 0.0)) * ConstantBufferPerFrame.directionLight.intensity.x) * ConstantBufferPerMaterial.mainColor.xyz, 1.0);
}

