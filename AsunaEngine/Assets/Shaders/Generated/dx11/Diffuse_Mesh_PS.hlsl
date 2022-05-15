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

static float4 _59;

uint2 spvTextureSize(Texture2D<float4> Tex, uint Level, out uint Param)
{
    uint2 ret;
    Tex.GetDimensions(Level, ret.x, ret.y, Param);
    return ret;
}

void frag_main()
{
    float _70 = max(dot(in_var_NORMAL.xyz, -ConstantBufferPerFrame_directionLight.direction.xyz), 0.0f);
    float4 _75 = tBaseMap.Sample(tBaseMapSamplerState, in_var_TEXCOORD0.xy);
    float _145;
    do
    {
        float3 _83 = in_var_TEXCOORD1.xyz / in_var_TEXCOORD1.w.xxx;
        float _84 = _83.x;
        float _88 = _83.y;
        float _93 = _83.z;
        if ((((((_84 < (-1.0f)) || (_84 > 1.0f)) || (_88 < (-1.0f))) || (_88 > 1.0f)) || (_93 < 0.0f)) || (_93 > 1.0f))
        {
            _145 = 0.0f;
            break;
        }
        float4 _102 = _59;
        _102.x = (_84 * 0.5f) + 0.5f;
        float4 _105 = _102;
        _105.y = (_88 * (-0.5f)) + 0.5f;
        float _108 = max(0.0500000007450580596923828125f * (1.0f - _70), 0.004999999888241291046142578125f);
        uint _109_dummy_parameter;
        uint2 _109 = spvTextureSize(tShadowMap, 0u, _109_dummy_parameter);
        float2 _115 = 1.0f.xx / float2(float(_109.x), float(_109.y));
        float _117;
        int _120;
        _117 = 0.0f;
        _120 = -1;
        float _118;
        for (; _120 <= 1; _117 = _118, _120++)
        {
            _118 = _117;
            for (int _128 = -1; _128 <= 1; )
            {
                _118 += (((_93 - _108) > tShadowMap.Sample(tShadowMapSamplerState, _105.xy + (float2(float(_120), float(_128)) * _115)).x) ? 0.199999988079071044921875f : 1.0f);
                _128++;
                continue;
            }
        }
        _145 = _117 * 0.111111111938953399658203125f;
        break;
    } while(false);
    out_var_SV_TARGET = float4((_75.xyz * min(((ConstantBufferPerFrame_directionLight.color.xyz * _70) * ConstantBufferPerFrame_directionLight.intensity.x) + 0.20000000298023223876953125f.xxx, 1.0f.xxx)) * _145, 1.0f);
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
