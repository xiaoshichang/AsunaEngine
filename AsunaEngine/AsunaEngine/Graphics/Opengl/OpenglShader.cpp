#include "OpenglShader.h"
#include "../../3rd/Glad/include/glad/glad.h"
#include "../../Foundation/Utils/FileHelper.h"
#include "../../Foundation/Platform/Assert.h"
using namespace asuna;
using namespace std;


OpenglShader::~OpenglShader()
{
	if (m_Shader != 0)
	{
		glDeleteShader(m_Shader);
		m_Shader = 0;
	}
}

std::shared_ptr<OpenglShader> asuna::OpenglShader::Create(const std::string& path, ShaderType shaderType)
{
	auto source = FileHelper::ReadText(path.c_str());
	auto sourceString = source.c_str();
	auto shader = glCreateShader(GL_VERTEX_SHADER);
	// Copy the shader source code strings into the vertex and fragment shader objects.
	{
		glShaderSource(shader, 1, &sourceString, NULL);
		glCompileShader(shader);
	}
	// Check to see if the vertex shader compiled successfully.
	{
		int status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status != 1)
		{
			ASUNA_ASSERT(false);
		}
	}
	return make_shared<OpenglShader>(shader, shaderType);
}

void OpenglShader::Bind()
{
}


OpenglShaderProgram::~OpenglShaderProgram()
{
	if (m_Program != 0)
	{
		glDeleteProgram(m_Program);
		m_Program = 0;
	}
}

shared_ptr<OpenglShaderProgram> OpenglShaderProgram::Create(const std::shared_ptr<OpenglShader>& vs, const std::shared_ptr<OpenglShader>& ps)
{
	// Create a shader program object.
	auto program = glCreateProgram();
	// Attach the vertex and fragment shader to the program object.
	glAttachShader(program, vs->GetShader());
	glAttachShader(program, ps->GetShader());
	// Bind the shader input variables.
	glBindAttribLocation(program, 0, "inputPosition");
	glBindAttribLocation(program, 1, "inputColor");
	// Link the shader program.
	glLinkProgram(program);

	// Check the status of the link.
	int status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status != 1)
	{
		// If it did not link then write the syntax error message out to a text file for review.
		return nullptr;
	}
	return make_shared<OpenglShaderProgram>(program);
}

