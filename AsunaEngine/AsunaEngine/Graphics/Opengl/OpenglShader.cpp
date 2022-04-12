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
    unsigned int shader = 0;
    if (shaderType == ShaderType::VertexShader)
    {
        shader = glCreateShader(GL_VERTEX_SHADER);
    }
    else if (shaderType == ShaderType::PixelShader)
    {
        shader = glCreateShader(GL_FRAGMENT_SHADER);
    }
    else
    {
        ASUNA_ASSERT(false);
    }
	// Copy the shader source code strings into the vertex and fragment shader objects.
	{
		glShaderSource(shader, 1, &sourceString, nullptr);
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

