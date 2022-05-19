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

static float4 _57;

uint2 spvTextureSize(Texture2D<float4> Tex, uint Level, out uint Param)
{
    uint2 ret;
    Tex.GetDimensions(Level, ret.x, ret.y, Param);
    return ret;
}

void frag_main()
{
    float4 _66 = tBaseMap.Sample(tBaseMapSamplerState, in_var_TEXCOORD0.xy);
    float _133;
    do
    {
        float3 _74 = in_var_TEXCOORD1.xyz / in_var_TEXCOORD1.w.xxx;
        float _75 = _74.x;
        float _79 = _74.y;
        float _84 = _74.z;
        if ((((((_75 < (-1.0f)) || (_75 > 1.0f)) || (_79 < (-1.0f))) || (_79 > 1.0f)) || (_84 < 0.0f)) || (_84 > 1.0f))
        {
            _133 = 0.0f;
            break;
        }
        float4 _93 = _57;
        _93.x = (_75 * 0.5f) + 0.5f;
        float4 _96 = _93;
        _96.y = (_79 * (-0.5f)) + 0.5f;
        uint _97_dummy_parameter;
        uint2 _97 = spvTextureSize(tShadowMap, 0u, _97_dummy_parameter);
        float2 _103 = 1.0f.xx / float2(float(_97.x), float(_97.y));
        float _105;
        int _108;
        _105 = 0.0f;
        _108 = -1;
        float _106;
        for (; _108 <= 1; _105 = _106, _108++)
        {
            _106 = _105;
            for (int _116 = -1; _116 <= 1; )
            {
                _106 += (((_84 - 0.00999999977648258209228515625f) > tShadowMap.Sample(tShadowMapSamplerState, _96.xy + (float2(float(_108), float(_116)) * _103)).x) ? 0.0f : 1.0f);
                _116++;
                continue;
            }
        }
        _133 = _105 * 0.111111111938953399658203125f;
        break;
    } while(false);
    out_var_SV_TARGET = float4(min(0.20000000298023223876953125f.xxx + (((ConstantBufferPerFrame_directionLight.color.xyz * max(dot(in_var_NORMAL.xyz, -ConstantBufferPerFrame_directionLight.direction.xyz), 0.0f)) * ConstantBufferPerFrame_directionLight.intensity.x) * _133), 1.0f.xxx) * _66.xyz, 1.0f);
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
