//
// Created by xiao on 2022/4/12.
//
#include "OpenglRenderer.h"
#include "OpenglConstantBuffer.h"


using namespace asuna;


OpenglConstantBuffer::OpenglConstantBuffer(ConstantBufferDataType dt, int size) :
        ConstantBuffer(dt, size)
{
    glGenBuffers(1, &m_Buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, m_Buffer);
    glBufferData(GL_UNIFORM_BUFFER, m_Size, nullptr, GL_DYNAMIC_DRAW);

}

std::shared_ptr<OpenglConstantBuffer> OpenglConstantBuffer::Create(ConstantBufferDataType dt, int size)
{
    return std::make_shared<OpenglConstantBuffer>(dt, size);
}

void OpenglConstantBuffer::Bind()
{
    if (m_DataType == ConstantBufferDataType::PerFrame)
    {
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_Buffer);
    }
    else if (m_DataType == ConstantBufferDataType::PerObject)
    {
        glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_Buffer);
    }
    else
    {
        glBindBufferBase(GL_UNIFORM_BUFFER, 2, m_Buffer);
    }

    auto* mapData = glMapBufferRange(GL_UNIFORM_BUFFER, 0, m_Size, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    memcpy(mapData, m_Data, m_Size);
    glUnmapBuffer(GL_UNIFORM_BUFFER);
}








