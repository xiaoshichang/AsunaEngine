#pragma once
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
	protected:
		DirectX11ConstantBuffer() :
			m_Data()
		{}
	public:
		ConstantBufferData m_Data;
		ID3D11Buffer* m_ConstantBuffer = nullptr;

	public:
		static DirectX11ConstantBuffer* Create();

	};

}

