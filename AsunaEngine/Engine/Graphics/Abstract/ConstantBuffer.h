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

    struct ConstantBufferDataDirectionLight
    {
        Vector4f m_Direction;
        Vector4f m_Color;
        Vector4f m_Intensity;
    };

    struct ConstantBufferDataSpotLight
    {
        Vector4f m_Direction;
        Vector4f m_Position;
        Vector4f m_Color;
        Vector4f m_Intensity;
    };

    struct ConstantBufferDataPointLight
    {
        Vector4f m_Position;
        Vector4f m_Color;
        Vector4f m_Intensity;
    };

    struct ConstantBufferDataPerFrame
    {
        Matrix4x4f m_ViewMatrix;
        Matrix4x4f m_ProjectionMatrix;
        Matrix4x4f m_VP;
        ConstantBufferDataDirectionLight m_DirectionLight;
        ConstantBufferDataSpotLight m_SpotLight[4];
        ConstantBufferDataPointLight m_PointLight[4];
        Matrix4x4f m_LightViewProj;
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