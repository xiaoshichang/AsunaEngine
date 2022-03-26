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
	};


	class DirectX11VextexShader : public DirectX11Shader
	{
	public:
		DirectX11VextexShader()
			: DirectX11Shader(ShaderType::VertexShader)
		{
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

	public:
		ID3D11PixelShader* m_PS = nullptr;

	public:
		static std::shared_ptr<DirectX11PixelShader> Create(const std::string& path);

	};
}

