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

layout(binding = 1, std140) uniform type_ConstantBufferPerObject
{
    layout(row_major) mat4 worldMatrix;
} ConstantBufferPerObject;

layout(binding = 2, std140) uniform type_ConstantBufferPerMaterial
{
    vec4 mainColor;
    layout(row_major) mat4 modelMatrix;
} ConstantBufferPerMaterial;

layout(location = 0) in vec3 in_var_POSITION;
layout(location = 1) in vec3 in_var_NORMAL;
layout(location = 0) out vec4 out_var_NORMAL;

mat4 spvWorkaroundRowMajor(mat4 wrap) { return wrap; }

void main()
{
    gl_Position = spvWorkaroundRowMajor(ConstantBufferPerFrame.projectionMatrix) * (spvWorkaroundRowMajor(ConstantBufferPerFrame.viewMatrix) * (spvWorkaroundRowMajor(ConstantBufferPerObject.worldMatrix) * (spvWorkaroundRowMajor(ConstantBufferPerMaterial.modelMatrix) * vec4(in_var_POSITION, 1.0))));
    out_var_NORMAL = normalize(spvWorkaroundRowMajor(ConstantBufferPerObject.worldMatrix) * normalize(spvWorkaroundRowMajor(ConstantBufferPerMaterial.modelMatrix) * vec4(in_var_NORMAL, 0.0)));
}

