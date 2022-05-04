#version 420

layout(binding = 2, std140) uniform type_ConstantBufferPerMaterial
{
    vec4 mainColor;
    layout(row_major) mat4 modelMatrix;
} ConstantBufferPerMaterial;

layout(location = 0) out vec4 out_var_SV_TARGET;

void main()
{
    out_var_SV_TARGET = ConstantBufferPerMaterial.mainColor;
}

