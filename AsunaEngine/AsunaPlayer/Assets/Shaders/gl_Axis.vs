////////////////////////////////////////////////////////////////////////////////
// Filename: color.vs
////////////////////////////////////////////////////////////////////////////////

#version 420

/////////////////////
// INPUT VARIABLES //
/////////////////////
in vec3 inputPosition;

//////////////////////
// OUTPUT VARIABLES //
//////////////////////

///////////////////////
// UNIFORM VARIABLES //
///////////////////////
layout (std140, binding = 0) uniform ConstantBufferPerFrame
{
    mat4 viewMatrix;
    mat4 projectionMatrix;
    mat4 vp;
};

layout (std140, binding = 1) uniform ConstantBufferPerObject
{
    mat4 worldMatrix;
};

layout (std140, binding = 2) uniform ConstantBufferPerMaterial
{
    vec4 mainColor;
    mat4 ModelMatrix;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
void main(void)
{
	// Calculate the position of the vertex against the world, view, and projection matrices.
	gl_Position = vec4(inputPosition, 1.0f);
	gl_Position =  gl_Position * viewMatrix;
	gl_Position =  gl_Position * projectionMatrix;
}

