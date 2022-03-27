#pragma once

namespace asuna
{
	enum class ShaderType
	{
		VertexShader = 1,
		PixelShader
	};


	class Shader
	{
	public:
		Shader(ShaderType shaderType) : 
			m_ShaderType(shaderType)
		{
		}

		virtual ~Shader()
		{
		}

	public:
		ShaderType m_ShaderType;
	};
}

