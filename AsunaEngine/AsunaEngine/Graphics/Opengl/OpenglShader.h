#pragma once
#include <memory>
#include <iostream>
#include "../Abstract/Shader.h"
namespace asuna
{
	////////////////////////////////////////////////////////////////////
	/////////////////////////// Basic //////////////////////////////////
	////////////////////////////////////////////////////////////////////
	class OpenglShader : public Shader
	{
	public:
		OpenglShader() = delete;
		OpenglShader(unsigned int shader, ShaderType shaderType) : 
			m_Shader(shader),
			Shader(shaderType)
		{
		}

		~OpenglShader() override;
		
		unsigned int GetShader() const
		{
			return m_Shader;
		}

        void Bind() override;

	private:
		unsigned int m_Shader = 0;

	public:
		static std::shared_ptr<OpenglShader> Create(const std::string& path, ShaderType shaderType);

	};


	////////////////////////////////////////////////////////////////////
	/////////////////////////// Shader Program /////////////////////////
	////////////////////////////////////////////////////////////////////
	class OpenglShaderProgram
	{
	public:
		OpenglShaderProgram() = delete;
		OpenglShaderProgram(unsigned int program):
			m_Program(program)
		{
		}

        ~OpenglShaderProgram();
		

	public:
		unsigned int m_Program = 0;
	public:
		static std::shared_ptr<OpenglShaderProgram> Create(const std::shared_ptr<OpenglShader>& vs, const std::shared_ptr<OpenglShader>& ps);
	};

}

