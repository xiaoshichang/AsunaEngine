#pragma once
#include <memory>
#include <d3d11.h>
#include "../ConstantBuffer.h"

namespace asuna
{
	
	class DirectX11ConstantBuffer : public ConstantBuffer
	{
	public:
		DirectX11ConstantBuffer() = delete;
		DirectX11ConstantBuffer(ID3D11Buffer* cb) :
			m_ConstantBuffer(cb),
			ConstantBuffer()
		{
		}

		virtual ~DirectX11ConstantBuffer()
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


	private:
		ID3D11Buffer* m_ConstantBuffer = nullptr;

	public:
		static std::shared_ptr<DirectX11ConstantBuffer> Create();

	};

}

