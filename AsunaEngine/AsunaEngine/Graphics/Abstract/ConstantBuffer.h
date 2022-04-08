#pragma once
#include "../../Foundation/Math/AMath.h"


namespace asuna
{
    enum class ConstantBufferDataType
    {
        PerMaterial,
        PerObject,
        PerFrame
    };

    struct ConstantBufferDataPerFrame
    {
        Matrix4x4f m_ViewMatrix;
        Matrix4x4f m_ProjectionMatrix;
        Matrix4x4f m_VP;
    };


	struct ConstantBufferDataPerObject
	{
		Matrix4x4f m_WorldMatrix;
	};


	class ConstantBuffer
	{
	public:
		explicit ConstantBuffer(ConstantBufferDataType dt);
		virtual ~ConstantBuffer();
		void* GetData()
		{
			return m_Data;
		}

        static int GetDataSizeByDataType(ConstantBufferDataType dt);


	private:
        ConstantBufferDataType m_DataType;
		void* m_Data = nullptr;

	};

}