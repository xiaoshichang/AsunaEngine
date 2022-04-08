#include "ConstantBuffer.h"

using namespace asuna;

ConstantBuffer::ConstantBuffer(ConstantBufferDataType dt)  :
        m_DataType(dt)
{
    if (ConstantBufferDataType::PerObject == dt)
    {
        m_Data = new ConstantBufferDataPerObject();
    }
    else if (ConstantBufferDataType::PerFrame == dt)
    {
        m_Data = new ConstantBufferDataPerFrame();
    }
    else
    {
        m_Data = nullptr;
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
        m_Data = nullptr;
    }
}

int ConstantBuffer::GetDataSizeByDataType(ConstantBufferDataType dt)
{
    switch (dt)
    {
        case ConstantBufferDataType::PerFrame : return sizeof(ConstantBufferDataPerFrame);
        case ConstantBufferDataType::PerObject : return sizeof(ConstantBufferDataPerObject);
        default:
            return 0;
    }
}




