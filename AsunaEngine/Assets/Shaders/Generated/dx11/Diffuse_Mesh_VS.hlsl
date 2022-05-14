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
static float3 in_var_NORMAL;
static float3 in_var_TEXCOORD;
static float4 out_var_NORMAL;
static float4 out_var_TEXCOORD0;
static float4 out_var_TEXCOORD1;

struct SPIRV_Cross_Input
{
    float3 in_var_POSITION : POSITION;
    float3 in_var_NORMAL : NORMAL;
    float3 in_var_TEXCOORD : TEXCOORD;
};

struct SPIRV_Cross_Output
{
    float4 out_var_NORMAL : TEXCOORD0;
    float4 out_var_TEXCOORD0 : TEXCOORD1;
    float4 out_var_TEXCOORD1 : TEXCOORD2;
    float4 gl_Position : SV_Position;
};

void vert_main()
{
    float4 _54 = mul(mul(float4(in_var_POSITION, 1.0f), ConstantBufferPerMaterial_modelMatrix), ConstantBufferPerObject_worldMatrix);
    gl_Position = mul(mul(_54, ConstantBufferPerFrame_viewMatrix), ConstantBufferPerFrame_projectionMatrix);
    out_var_NORMAL = normalize(mul(normalize(mul(float4(in_var_NORMAL, 0.0f), ConstantBufferPerMaterial_modelMatrix)), ConstantBufferPerObject_worldMatrix));
    out_var_TEXCOORD0 = float4(in_var_TEXCOORD, 1.0f);
    out_var_TEXCOORD1 = mul(_54, ConstantBufferPerFrame_lightViewProj);
}

SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
{
    in_var_POSITION = stage_input.in_var_POSITION;
    in_var_NORMAL = stage_input.in_var_NORMAL;
    in_var_TEXCOORD = stage_input.in_var_TEXCOORD;
    vert_main();
    SPIRV_Cross_Output stage_output;
    stage_output.gl_Position = gl_Position;
    stage_output.out_var_NORMAL = out_var_NORMAL;
    stage_output.out_var_TEXCOORD0 = out_var_TEXCOORD0;
    stage_output.out_var_TEXCOORD1 = out_var_TEXCOORD1;
    return stage_output;
}
