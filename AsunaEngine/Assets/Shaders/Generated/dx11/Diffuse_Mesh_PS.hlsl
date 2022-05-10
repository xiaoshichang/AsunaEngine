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

Texture2D<float4> tBaseMap : register(t0);
SamplerState tBaseMapSamplerState : register(s0);

static float4 in_var_NORMAL;
static float4 in_var_TEXCOORD;
static float4 out_var_SV_TARGET;

struct SPIRV_Cross_Input
{
    float4 in_var_NORMAL : TEXCOORD0;
    float4 in_var_TEXCOORD : TEXCOORD1;
};

struct SPIRV_Cross_Output
{
    float4 out_var_SV_TARGET : SV_Target0;
};

void frag_main()
{
    out_var_SV_TARGET = float4(tBaseMap.Sample(tBaseMapSamplerState, in_var_TEXCOORD.xy).xyz * min(((ConstantBufferPerFrame_directionLight.color.xyz * max(dot(in_var_NORMAL.xyz, -ConstantBufferPerFrame_directionLight.direction.xyz), 0.0f)) * ConstantBufferPerFrame_directionLight.intensity.x) + 0.20000000298023223876953125f.xxx, 1.0f.xxx), 1.0f);
}

SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
{
    in_var_NORMAL = stage_input.in_var_NORMAL;
    in_var_TEXCOORD = stage_input.in_var_TEXCOORD;
    frag_main();
    SPIRV_Cross_Output stage_output;
    stage_output.out_var_SV_TARGET = out_var_SV_TARGET;
    return stage_output;
}
