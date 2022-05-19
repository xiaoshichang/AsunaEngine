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

static float4 _57;

uint2 spvTextureSize(Texture2D<float4> Tex, uint Level, out uint Param)
{
    uint2 ret;
    Tex.GetDimensions(Level, ret.x, ret.y, Param);
    return ret;
}

void frag_main()
{
    float _127;
    do
    {
        float3 _68 = in_var_TEXCOORD0.xyz / in_var_TEXCOORD0.w.xxx;
        float _69 = _68.x;
        float _73 = _68.y;
        float _78 = _68.z;
        if ((((((_69 < (-1.0f)) || (_69 > 1.0f)) || (_73 < (-1.0f))) || (_73 > 1.0f)) || (_78 < 0.0f)) || (_78 > 1.0f))
        {
            _127 = 0.0f;
            break;
        }
        float4 _87 = _57;
        _87.x = (_69 * 0.5f) + 0.5f;
        float4 _90 = _87;
        _90.y = (_73 * (-0.5f)) + 0.5f;
        uint _91_dummy_parameter;
        uint2 _91 = spvTextureSize(tShadowMap, 0u, _91_dummy_parameter);
        float2 _97 = 1.0f.xx / float2(float(_91.x), float(_91.y));
        float _99;
        int _102;
        _99 = 0.0f;
        _102 = -1;
        float _100;
        for (; _102 <= 1; _99 = _100, _102++)
        {
            _100 = _99;
            for (int _110 = -1; _110 <= 1; )
            {
                _100 += (((_78 - 0.00999999977648258209228515625f) > tShadowMap.Sample(tShadowMapSamplerState, _90.xy + (float2(float(_102), float(_110)) * _97)).x) ? 0.0f : 1.0f);
                _110++;
                continue;
            }
        }
        _127 = _99 * 0.111111111938953399658203125f;
        break;
    } while(false);
    out_var_SV_TARGET = float4(min(0.20000000298023223876953125f.xxx + (((ConstantBufferPerFrame_directionLight.color.xyz * max(dot(in_var_NORMAL.xyz, -ConstantBufferPerFrame_directionLight.direction.xyz), 0.0f)) * ConstantBufferPerFrame_directionLight.intensity.x) * _127), 1.0f.xxx) * ConstantBufferPerMaterial_mainColor.xyz, 1.0f);
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
