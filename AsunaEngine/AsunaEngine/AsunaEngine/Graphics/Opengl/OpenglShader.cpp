#include "OpenglShader.h"
#include "glad/glad.h"
#include "../../Foundation/Utils/FileHelper.h"
#include "../../Foundation/Platform/Assert.h"
using namespace asuna;



OpenglVertexShader* OpenglVertexShader::Create(const std::string& path)
{
	OpenglVertexShader* shader = new OpenglVertexShader();
	auto source = FileHelper::ReadText(path.c_str());
	auto sourceString = source.c_str();
	shader->m_Shader = glCreateShader(GL_VERTEX_SHADER);
	// Copy the shader source code strings into the vertex and fragment shader objects.
	{
		glShaderSource(shader->m_Shader, 1, &sourceString, NULL);
		glCompileShader(shader->m_Shader);
	}
	// Check to see if the vertex shader compiled successfully.
	{
		int status;
		glGetShaderiv(shader->m_Shader, GL_COMPILE_STATUS, &status);
		if (status != 1)
		{
			ASUNA_ASSERT(false);
		}
	}
	return shader;
}

OpenglPixelShader* OpenglPixelShader::Create(const std::string& path)
{
	OpenglPixelShader* shader = new OpenglPixelShader();
	auto source = FileHelper::ReadText(path.c_str());
	auto sourceString = source.c_str();
	shader->m_Shader = glCreateShader(GL_FRAGMENT_SHADER);
	// Copy the shader source code strings into the vertex and fragment shader objects.
	{
		glShaderSource(shader->m_Shader, 1, &sourceString, NULL);
		glCompileShader(shader->m_Shader);
	}
	// Check to see if the vertex shader compiled successfully.
	{
		int status;
		glGetShaderiv(shader->m_Shader, GL_COMPILE_STATUS, &status);
		if (status != 1)
		{
			ASUNA_ASSERT(false);
		}
	}
	return shader;
}

OpenglShaderProgram* OpenglShaderProgram::Create(OpenglVertexShader* vs, OpenglPixelShader* ps)
{
	OpenglShaderProgram* shaderProgram = new OpenglShaderProgram();
	// Create a shader program object.
	shaderProgram->m_Program = glCreateProgram();

	// Attach the vertex and fragment shader to the program object.
	glAttachShader(shaderProgram->m_Program, vs->m_Shader);
	glAttachShader(shaderProgram->m_Program, ps->m_Shader);

	// Bind the shader input variables.
	glBindAttribLocation(shaderProgram->m_Program, 0, "inputPosition");
	glBindAttribLocation(shaderProgram->m_Program, 1, "inputColor");

	// Link the shader program.
	glLinkProgram(shaderProgram->m_Program);

	// Check the status of the link.
	int status;
	glGetProgramiv(shaderProgram->m_Program, GL_LINK_STATUS, &status);
	if (status != 1)
	{
		// If it did not link then write the syntax error message out to a text file for review.
		return nullptr;
	}
	return shaderProgram;
}
