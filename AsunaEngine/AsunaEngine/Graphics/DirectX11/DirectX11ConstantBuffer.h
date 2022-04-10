#pragma once
#include <memory>
#include <d3d11.h>
#include "../Abstract/ConstantBuffer.h"

namespace asuna
{
	
	class DirectX11ConstantBuffer : public ConstantBuffer
	{
	public:
		DirectX11ConstantBuffer() = delete;
		explicit DirectX11ConstantBuffer(ConstantBufferDataType dt, int size, ID3D11Buffer* cb) :
			m_ConstantBuffer(cb),
			ConstantBuffer(dt, size)
		{
		}

		~DirectX11ConstantBuffer() override
		{
			if (m_ConstantBuffer != nullptr)
			{
				m_ConstantBuffer->Release();
				m_ConstantBuffer = nullptr;
			}
		}

		ID3D11Buffer* GetBuffer()
		{
			return m_ConstantBuffer;
		}

        void Bind() override;


	private:
		ID3D11Buffer* m_ConstantBuffer = nullptr;

	public:
		static std::shared_ptr<DirectX11ConstantBuffer> Create(ConstantBufferDataType dt, int size);

	};

}

