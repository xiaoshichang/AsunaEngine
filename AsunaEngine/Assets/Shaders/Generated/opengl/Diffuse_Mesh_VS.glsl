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
layout(location = 2) in vec3 in_var_TEXCOORD;
layout(location = 0) out vec4 out_var_NORMAL;
layout(location = 1) out vec4 out_var_TEXCOORD0;
layout(location = 2) out vec4 out_var_TEXCOORD1;

mat4 spvWorkaroundRowMajor(mat4 wrap) { return wrap; }

void main()
{
    vec4 _54 = spvWorkaroundRowMajor(ConstantBufferPerObject.worldMatrix) * (spvWorkaroundRowMajor(ConstantBufferPerMaterial.modelMatrix) * vec4(in_var_POSITION, 1.0));
    gl_Position = spvWorkaroundRowMajor(ConstantBufferPerFrame.projectionMatrix) * (spvWorkaroundRowMajor(ConstantBufferPerFrame.viewMatrix) * _54);
    out_var_NORMAL = normalize(spvWorkaroundRowMajor(ConstantBufferPerObject.worldMatrix) * normalize(spvWorkaroundRowMajor(ConstantBufferPerMaterial.modelMatrix) * vec4(in_var_NORMAL, 0.0)));
    out_var_TEXCOORD0 = vec4(in_var_TEXCOORD, 1.0);
    out_var_TEXCOORD1 = spvWorkaroundRowMajor(ConstantBufferPerFrame.lightViewProj) * _54;
}

