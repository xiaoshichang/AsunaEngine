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
		DirectX11VertexShader(ID3D11VertexShader* vs, ID3D11InputLayout* layout, ID3DBlob* bytecode) :
			m_VS(vs),
			m_Layout(layout),
            m_ByteCode(bytecode),
			Shader(ShaderType::VertexShader)
		{
		}

		~DirectX11VertexShader() override
		{
            if (m_ByteCode != nullptr)
            {
                m_ByteCode->Release();
                m_ByteCode = nullptr;
            }
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

        void Bind() override;


	private:
		ID3D11VertexShader* m_VS;
		ID3D11InputLayout* m_Layout;
        ID3DBlob* m_ByteCode;

	public:
		static std::shared_ptr<DirectX11VertexShader> Create(const std::string& path);
	};


	class DirectX11PixelShader : public Shader
	{
	public:
		explicit DirectX11PixelShader(ID3D11PixelShader* ps, ID3DBlob* bytecode) :
			m_PS(ps),
            m_ByteCode(bytecode),
			Shader(ShaderType::PixelShader)
		{
		}

		~DirectX11PixelShader() override
		{
            if (m_ByteCode != nullptr)
            {
                m_ByteCode->Release();
                m_ByteCode = nullptr;
            }
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

        void Bind() override;


	private:
		ID3D11PixelShader* m_PS = nullptr;
        ID3DBlob* m_ByteCode;

	public:
		static std::shared_ptr<DirectX11PixelShader> Create(const std::string& path);

	};
}

