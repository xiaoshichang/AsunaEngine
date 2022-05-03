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
		explicit Shader(ShaderType shaderType) :
			m_ShaderType(shaderType)
		{
		}

		virtual ~Shader() = default;

        virtual void Bind() = 0;

	public:
		ShaderType m_ShaderType;
	};
}

