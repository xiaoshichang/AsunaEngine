#pragma once
#include <d3d11.h>
#include <memory>
#include <iostream>
#include "../Abstract/Shader.h"

namespace asuna
{

	class DirectX11VertexShader : public Shader
	{
	public:
		DirectX11VertexShader() = delete;
		DirectX11VertexShader(ID3D11VertexShader* vs, ID3D11InputLayout* layout) :
			m_VS(vs),
			m_Layout(layout),
			Shader(ShaderType::VertexShader)
		{
		}

		virtual ~DirectX11VertexShader()
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

		ID3D11VertexShader* GetShader()
		{
			return m_VS;
		}

		ID3D11InputLayout* GetLayout()
		{
			return m_Layout;
		}


	private:
		ID3D11VertexShader* m_VS;
		ID3D11InputLayout* m_Layout;              

	public:
		static std::shared_ptr<DirectX11VertexShader> Create(const std::string& path);
	};


	class DirectX11PixelShader : public Shader
	{
	public:
		DirectX11PixelShader() = delete;
		DirectX11PixelShader(ID3D11PixelShader* ps) :
			m_PS(ps),
			Shader(ShaderType::PixelShader)
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

		ID3D11PixelShader* GetShader()
		{
			return m_PS;
		}


	private:
		ID3D11PixelShader* m_PS = nullptr;

	public:
		static std::shared_ptr<DirectX11PixelShader> Create(const std::string& path);

	};
}

