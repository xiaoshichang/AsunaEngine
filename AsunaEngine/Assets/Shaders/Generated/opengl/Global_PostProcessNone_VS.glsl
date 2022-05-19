#version 420

layout(location = 0) in vec3 in_var_POSITION;
layout(location = 1) in vec3 in_var_TEXCOORD;
layout(location = 0) out vec4 out_var_TEXCOORD;

void main()
{
    gl_Position = vec4(in_var_POSITION, 1.0);
    out_var_TEXCOORD = vec4(in_var_TEXCOORD, 1.0);
}

