Texture2D<float4> tBaseMap : register(t0);
SamplerState tBaseMapSamplerState : register(s0);

static float4 in_var_TEXCOORD;
static float4 out_var_SV_TARGET;

struct SPIRV_Cross_Input
{
    float4 in_var_TEXCOORD : TEXCOORD0;
};

struct SPIRV_Cross_Output
{
    float4 out_var_SV_TARGET : SV_Target0;
};

void frag_main()
{
    out_var_SV_TARGET = float4(tBaseMap.Sample(tBaseMapSamplerState, in_var_TEXCOORD.xy).xxx, 1.0f);
}

SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
{
    in_var_TEXCOORD = stage_input.in_var_TEXCOORD;
    frag_main();
    SPIRV_Cross_Output stage_output;
    stage_output.out_var_SV_TARGET = out_var_SV_TARGET;
    return stage_output;
}
