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
		explicit ConstantBuffer(ConstantBufferDataType dt, int size);
		virtual ~ConstantBuffer();
		void* GetData()
		{
			return m_Data;
		}

        ConstantBufferDataType GetDataType()
        {
            return m_DataType;
        }

        virtual void Bind() = 0;


	protected:
        ConstantBufferDataType m_DataType;
        int m_Size;
		void* m_Data = nullptr;

	};

}