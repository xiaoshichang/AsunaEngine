#pragma once
#include <memory>
#include <iostream>
#include "../Shader.h"
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

		virtual ~OpenglShader();
		
		unsigned int GetShader()
		{
			return m_Shader;
		}

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

		virtual ~OpenglShaderProgram();
		

	public:
		unsigned int m_Program = 0;
	public:
		static std::shared_ptr<OpenglShaderProgram> Create(std::shared_ptr<OpenglShader> vs, std::shared_ptr<OpenglShader> ps);
	};

}

