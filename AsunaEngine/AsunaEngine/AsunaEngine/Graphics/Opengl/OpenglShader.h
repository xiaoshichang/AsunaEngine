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
		OpenglShader(ShaderType shaderType) : Shader(shaderType)
		{
		}

		virtual ~OpenglShader();
		

	public:
		unsigned int m_Shader = 0;

	};

	////////////////////////////////////////////////////////////////////
	/////////////////////////// Vertex Shader //////////////////////////
	////////////////////////////////////////////////////////////////////

	class OpenglVertexShader : public OpenglShader
	{
	public:
		OpenglVertexShader() : OpenglShader(ShaderType::VertexShader)
		{
		}

		virtual ~OpenglVertexShader();
		

	public:
		static std::shared_ptr<OpenglVertexShader> Create(const std::string& path);
	};

	////////////////////////////////////////////////////////////////////
	/////////////////////////// Pixel Shader ///////////////////////////
	////////////////////////////////////////////////////////////////////
	class OpenglPixelShader : public OpenglShader
	{
	public:
		OpenglPixelShader() : OpenglShader(ShaderType::PixelShader)
		{
		}

		virtual ~OpenglPixelShader();
		

	public:
		static std::shared_ptr<OpenglPixelShader> Create(const std::string& path);
	};


	////////////////////////////////////////////////////////////////////
	/////////////////////////// Shader Program /////////////////////////
	////////////////////////////////////////////////////////////////////
	class OpenglShaderProgram
	{
	public:
		OpenglShaderProgram()
		{
		}

		virtual ~OpenglShaderProgram();
		

	public:
		unsigned int m_Program = 0;
	public:
		static std::shared_ptr<OpenglShaderProgram> Create(std::shared_ptr<OpenglVertexShader> vs, std::shared_ptr<OpenglPixelShader> ps);
	};

}

