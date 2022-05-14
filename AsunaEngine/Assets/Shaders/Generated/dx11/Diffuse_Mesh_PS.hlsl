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
Texture2D<float4> tShadowMap : register(t1);
SamplerState tShadowMapSamplerState : register(s1);

static float4 in_var_NORMAL;
static float4 in_var_TEXCOORD0;
static float4 in_var_TEXCOORD1;
static float4 out_var_SV_TARGET;

struct SPIRV_Cross_Input
{
    float4 in_var_NORMAL : TEXCOORD0;
    float4 in_var_TEXCOORD0 : TEXCOORD1;
    float4 in_var_TEXCOORD1 : TEXCOORD2;
};

struct SPIRV_Cross_Output
{
    float4 out_var_SV_TARGET : SV_Target0;
};

static float4 _52;

void frag_main()
{
    float4 _61 = tBaseMap.Sample(tBaseMapSamplerState, in_var_TEXCOORD0.xy);
    float _99;
    do
    {
        float3 _69 = in_var_TEXCOORD1.xyz / in_var_TEXCOORD1.w.xxx;
        float _70 = _69.x;
        float _74 = _69.y;
        float _79 = _69.z;
        if ((((((_70 < (-1.0f)) || (_70 > 1.0f)) || (_74 < (-1.0f))) || (_74 > 1.0f)) || (_79 < 0.0f)) || (_79 > 1.0f))
        {
            _99 = 0.0f;
            break;
        }
        float4 _88 = _52;
        _88.x = (_70 * 0.5f) + 0.5f;
        float4 _91 = _88;
        _91.y = (_74 * (-0.5f)) + 0.5f;
        if (_79 > tShadowMap.Sample(tShadowMapSamplerState, _91.xy).x)
        {
            _99 = 0.5f;
            break;
        }
        _99 = 1.0f;
        break;
    } while(false);
    out_var_SV_TARGET = float4((_61.xyz * min(((ConstantBufferPerFrame_directionLight.color.xyz * max(dot(in_var_NORMAL.xyz, -ConstantBufferPerFrame_directionLight.direction.xyz), 0.0f)) * ConstantBufferPerFrame_directionLight.intensity.x) + 0.20000000298023223876953125f.xxx, 1.0f.xxx)) * _99, 1.0f);
}

SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
{
    in_var_NORMAL = stage_input.in_var_NORMAL;
    in_var_TEXCOORD0 = stage_input.in_var_TEXCOORD0;
    in_var_TEXCOORD1 = stage_input.in_var_TEXCOORD1;
    frag_main();
    SPIRV_Cross_Output stage_output;
    stage_output.out_var_SV_TARGET = out_var_SV_TARGET;
    return stage_output;
}
