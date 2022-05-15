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

cbuffer type_ConstantBufferPerMaterial : register(b2)
{
    float4 ConstantBufferPerMaterial_mainColor : packoffset(c0);
    column_major float4x4 ConstantBufferPerMaterial_modelMatrix : packoffset(c1);
};

Texture2D<float4> tShadowMap : register(t1);
SamplerState tShadowMapSamplerState : register(s1);

static float4 in_var_NORMAL;
static float4 in_var_TEXCOORD0;
static float4 out_var_SV_TARGET;

struct SPIRV_Cross_Input
{
    float4 in_var_NORMAL : TEXCOORD0;
    float4 in_var_TEXCOORD0 : TEXCOORD1;
};

struct SPIRV_Cross_Output
{
    float4 out_var_SV_TARGET : SV_Target0;
};

static float4 _56;

uint2 spvTextureSize(Texture2D<float4> Tex, uint Level, out uint Param)
{
    uint2 ret;
    Tex.GetDimensions(Level, ret.x, ret.y, Param);
    return ret;
}

void frag_main()
{
    float _66 = max(dot(in_var_NORMAL.xyz, -ConstantBufferPerFrame_directionLight.direction.xyz), 0.0f);
    float _136;
    do
    {
        float3 _74 = in_var_TEXCOORD0.xyz / in_var_TEXCOORD0.w.xxx;
        float _75 = _74.x;
        float _79 = _74.y;
        float _84 = _74.z;
        if ((((((_75 < (-1.0f)) || (_75 > 1.0f)) || (_79 < (-1.0f))) || (_79 > 1.0f)) || (_84 < 0.0f)) || (_84 > 1.0f))
        {
            _136 = 0.0f;
            break;
        }
        float4 _93 = _56;
        _93.x = (_75 * 0.5f) + 0.5f;
        float4 _96 = _93;
        _96.y = (_79 * (-0.5f)) + 0.5f;
        float _99 = max(0.0500000007450580596923828125f * (1.0f - _66), 0.004999999888241291046142578125f);
        uint _100_dummy_parameter;
        uint2 _100 = spvTextureSize(tShadowMap, 0u, _100_dummy_parameter);
        float2 _106 = 1.0f.xx / float2(float(_100.x), float(_100.y));
        float _108;
        int _111;
        _108 = 0.0f;
        _111 = -1;
        float _109;
        for (; _111 <= 1; _108 = _109, _111++)
        {
            _109 = _108;
            for (int _119 = -1; _119 <= 1; )
            {
                _109 += (((_84 - _99) > tShadowMap.Sample(tShadowMapSamplerState, _96.xy + (float2(float(_111), float(_119)) * _106)).x) ? 0.199999988079071044921875f : 1.0f);
                _119++;
                continue;
            }
        }
        _136 = _108 * 0.111111111938953399658203125f;
        break;
    } while(false);
    out_var_SV_TARGET = float4((((ConstantBufferPerFrame_directionLight.color.xyz * _66) * ConstantBufferPerFrame_directionLight.intensity.x) * ConstantBufferPerMaterial_mainColor.xyz) * _136, 1.0f);
}

SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
{
    in_var_NORMAL = stage_input.in_var_NORMAL;
    in_var_TEXCOORD0 = stage_input.in_var_TEXCOORD0;
    frag_main();
    SPIRV_Cross_Output stage_output;
    stage_output.out_var_SV_TARGET = out_var_SV_TARGET;
    return stage_output;
}
