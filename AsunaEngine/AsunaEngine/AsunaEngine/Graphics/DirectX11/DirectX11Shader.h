#pragma once
#include <d3d11.h>
#include <memory>
#include <iostream>
#include "../Shader.h"

namespace asuna
{
	class DirectX11Shader : public Shader
	{
	public:
		DirectX11Shader(ShaderType shaderType) : Shader(shaderType)
		{
		}

		virtual ~DirectX11Shader()
		{
		}

	};


	class DirectX11VextexShader : public DirectX11Shader
	{
	public:
		DirectX11VextexShader()
			: DirectX11Shader(ShaderType::VertexShader)
		{
		}

		virtual ~DirectX11VextexShader()
		{
			if (m_VS != nullptr)
			{
				m_VS->Release();
				m_VS = nullptr;
			}

			if (m_Layout != nullptr)
			{
				m_Layout->Release();
				m_Layout = nullptr;
			}
		}

	public:
		ID3D11VertexShader* m_VS = nullptr;
		ID3D11InputLayout* m_Layout = nullptr;              

	public:
		static std::shared_ptr<DirectX11VextexShader> Create(const std::string& path);
	};


	class DirectX11PixelShader : public DirectX11Shader
	{
	public:
		DirectX11PixelShader()
			: DirectX11Shader(ShaderType::PixelShader)
		{
		}

		virtual ~DirectX11PixelShader()
		{
			if (m_PS != nullptr)
			{
				m_PS->Release();
				m_PS = nullptr;
			}
		}

	public:
		ID3D11PixelShader* m_PS = nullptr;

	public:
		static std::shared_ptr<DirectX11PixelShader> Create(const std::string& path);

	};
}

