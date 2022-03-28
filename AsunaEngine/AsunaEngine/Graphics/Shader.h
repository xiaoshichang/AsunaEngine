#pragma once

namespace asuna
{
	enum class ShaderType
	{
		Unknown = 0,
		VertexShader,
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

