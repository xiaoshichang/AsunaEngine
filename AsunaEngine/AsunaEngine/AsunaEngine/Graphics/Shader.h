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
	protected:
		Shader(ShaderType shaderType) : 
			m_ShaderType(shaderType)
		{

		}

	public:
		ShaderType m_ShaderType;
	};
}

