////////////////////////////////////////////////////////////////////////////////
// Filename: color.vs
////////////////////////////////////////////////////////////////////////////////

#version 400

/////////////////////
// INPUT VARIABLES //
/////////////////////
in vec3 inputPosition;
in vec3 inputNormal;
in vec3 inputTexcoord0;

//////////////////////
// OUTPUT VARIABLES //
//////////////////////
out vec3 nw;
out vec3 texcoord0;

///////////////////////
// UNIFORM VARIABLES //
///////////////////////
uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 ModelMatrix;

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
void main(void)
{
	// Calculate the position of the vertex against the world, view, and projection matrices.
	gl_Position = ModelMatrix * vec4(inputPosition, 1.0f);
	gl_Position = worldMatrix * gl_Position;
	gl_Position = viewMatrix * gl_Position;
	gl_Position = projectionMatrix * gl_Position;

	// Store the input color for the pixel shader to use.
	nw = (worldMatrix * vec4(inputNormal, 0.0f)).xyz;
	nw = normalize(nw);
	texcoord0 = inputTexcoord0;
}

