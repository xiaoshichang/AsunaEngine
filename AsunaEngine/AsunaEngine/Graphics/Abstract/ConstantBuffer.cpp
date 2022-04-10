#include "ConstantBuffer.h"
#include "../../Foundation/Platform/Assert.h"
using namespace asuna;

ConstantBuffer::ConstantBuffer(ConstantBufferDataType dt, int size)  :
        m_DataType(dt)
{
    if (ConstantBufferDataType::PerObject == dt)
    {
        m_Data = new ConstantBufferDataPerObject();
        m_Size = sizeof(ConstantBufferDataPerObject);
    }
    else if (ConstantBufferDataType::PerFrame == dt)
    {
        m_Data = new ConstantBufferDataPerFrame();
        m_Size = sizeof(ConstantBufferDataPerFrame);
    }
    else
    {
        m_Data = new char[size];
        Vector4f red(1.0f, 1.0f, 0.0f, 1.0f);
        memcpy(m_Data, &red, sizeof(Vector4f));
        m_Size = size;
    }
}

ConstantBuffer::~ConstantBuffer()
{
    if (m_Data != nullptr)
    {
        if (ConstantBufferDataType::PerObject == m_DataType)
        {
            auto data = (ConstantBufferDataPerObject*)m_Data;
            delete data;
        }
        else if(ConstantBufferDataType::PerFrame == m_DataType)
        {
            auto data = (ConstantBufferDataPerFrame*)m_Data;
            delete data;
        }
        else if (ConstantBufferDataType::PerMaterial == m_DataType)
        {
            auto data = (char*)m_Data;
            delete[] data;
        }
        else
        {
            ASUNA_ASSERT(false);
        }
        m_Data = nullptr;
    }
}




