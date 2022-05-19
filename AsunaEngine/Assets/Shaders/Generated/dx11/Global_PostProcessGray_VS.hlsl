static float4 gl_Position;
static float3 in_var_POSITION;
static float3 in_var_TEXCOORD;
static float4 out_var_TEXCOORD;

struct SPIRV_Cross_Input
{
    float3 in_var_POSITION : POSITION;
    float3 in_var_TEXCOORD : TEXCOORD;
};

struct SPIRV_Cross_Output
{
    float4 out_var_TEXCOORD : TEXCOORD0;
    float4 gl_Position : SV_Position;
};

void vert_main()
{
    gl_Position = float4(in_var_POSITION, 1.0f);
    out_var_TEXCOORD = float4(in_var_TEXCOORD, 1.0f);
}

SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
{
    in_var_POSITION = stage_input.in_var_POSITION;
    in_var_TEXCOORD = stage_input.in_var_TEXCOORD;
    vert_main();
    SPIRV_Cross_Output stage_output;
    stage_output.gl_Position = gl_Position;
    stage_output.out_var_TEXCOORD = out_var_TEXCOORD;
    return stage_output;
}
