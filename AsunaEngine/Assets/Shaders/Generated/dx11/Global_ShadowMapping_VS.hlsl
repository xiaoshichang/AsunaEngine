struct DirectionLight
{
    float4 direction;
    float4 color;
    float4 intensity;
};

struct SpotLight
{
    float4 direction;
    float4 position;
    float4 color;
    float4 intensity;
};

struct PointLight
{
    float4 position;
    float4 color;
    float4 intensity;
};

cbuffer type_ConstantBufferPerFrame : register(b0)
{
    column_major float4x4 ConstantBufferPerFrame_viewMatrix : packoffset(c0);
    column_major float4x4 ConstantBufferPerFrame_projectionMatrix : packoffset(c4);
    column_major float4x4 ConstantBufferPerFrame_vp : packoffset(c8);
    DirectionLight ConstantBufferPerFrame_directionLight : packoffset(c12);
    SpotLight ConstantBufferPerFrame_spotLights[4] : packoffset(c15);
    PointLight ConstantBufferPerFrame_pointLights[4] : packoffset(c31);
    column_major float4x4 ConstantBufferPerFrame_lightViewProj : packoffset(c43);
};

cbuffer type_ConstantBufferPerObject : register(b1)
{
    column_major float4x4 ConstantBufferPerObject_worldMatrix : packoffset(c0);
};

cbuffer type_ConstantBufferPerMaterial : register(b2)
{
    float4 ConstantBufferPerMaterial_mainColor : packoffset(c0);
    column_major float4x4 ConstantBufferPerMaterial_modelMatrix : packoffset(c1);
};


static float4 gl_Position;
static float3 in_var_POSITION;

struct SPIRV_Cross_Input
{
    float3 in_var_POSITION : POSITION;
};

struct SPIRV_Cross_Output
{
    float4 gl_Position : SV_Position;
};

void vert_main()
{
    gl_Position = mul(mul(mul(float4(in_var_POSITION, 1.0f), ConstantBufferPerMaterial_modelMatrix), ConstantBufferPerObject_worldMatrix), ConstantBufferPerFrame_lightViewProj);
}

SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
{
    in_var_POSITION = stage_input.in_var_POSITION;
    vert_main();
    SPIRV_Cross_Output stage_output;
    stage_output.gl_Position = gl_Position;
    return stage_output;
}
