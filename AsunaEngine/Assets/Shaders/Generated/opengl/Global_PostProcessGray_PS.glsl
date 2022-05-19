#version 420

layout(binding = 0) uniform sampler2D SPIRV_Cross_CombinedtBaseMaptBaseMapSamplerState;

layout(location = 0) in vec4 in_var_TEXCOORD;
layout(location = 0) out vec4 out_var_SV_TARGET;

void main()
{
    out_var_SV_TARGET = vec4(texture(SPIRV_Cross_CombinedtBaseMaptBaseMapSamplerState, in_var_TEXCOORD.xy).xxx, 1.0);
}

