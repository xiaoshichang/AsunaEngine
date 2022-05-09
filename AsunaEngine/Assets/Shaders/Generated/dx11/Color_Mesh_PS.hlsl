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
};

cbuffer type_ConstantBufferPerMaterial : register(b2)
{
    float4 ConstantBufferPerMaterial_mainColor : packoffset(c0);
    column_major float4x4 ConstantBufferPerMaterial_modelMatrix : packoffset(c1);
};


static float4 in_var_NORMAL;
static float4 out_var_SV_TARGET;

struct SPIRV_Cross_Input
{
    float4 in_var_NORMAL : TEXCOORD0;
};

struct SPIRV_Cross_Output
{
    float4 out_var_SV_TARGET : SV_Target0;
};

void frag_main()
{
    out_var_SV_TARGET = float4(((ConstantBufferPerFrame_directionLight.color.xyz * max(dot(in_var_NORMAL.xyz, -ConstantBufferPerFrame_directionLight.direction.xyz), 0.0f)) * ConstantBufferPerFrame_directionLight.intensity.x) * ConstantBufferPerMaterial_mainColor.xyz, 1.0f);
}

SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
{
    in_var_NORMAL = stage_input.in_var_NORMAL;
    frag_main();
    SPIRV_Cross_Output stage_output;
    stage_output.out_var_SV_TARGET = out_var_SV_TARGET;
    return stage_output;
}
