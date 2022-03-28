#pragma once
#include "../Foundation/Math/AMath.h"


namespace asuna
{
	struct ConstantBufferData
	{
		Matrix4x4f m_WorldMatrix;
		Matrix4x4f m_ViexMatrix;
		Matrix4x4f m_ProjectionMatrix;
	};


	class ConstantBuffer
	{
	public:
		ConstantBuffer() :
			m_Data()
		{
		}

		virtual ~ConstantBuffer()
		{
		}

		ConstantBufferData& GetData()
		{
			return m_Data;
		}

	private:
		ConstantBufferData m_Data;

	};

}