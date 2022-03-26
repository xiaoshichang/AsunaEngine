#pragma once
#include <memory>
#include <d3d11.h>
#include "../../Foundation/Math/AMath.h"

namespace asuna
{
	struct ConstantBufferData
	{
		Matrix4x4f m_WorldMatrix;
		Matrix4x4f m_ViexMatrix;
		Matrix4x4f m_ProjectionMatrix;
	};

	class DirectX11ConstantBuffer
	{
	public:
		DirectX11ConstantBuffer() :
			m_Data()
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

	public:
		ConstantBufferData m_Data;
		ID3D11Buffer* m_ConstantBuffer = nullptr;

	public:
		static std::shared_ptr<DirectX11ConstantBuffer> Create();

	};

}

