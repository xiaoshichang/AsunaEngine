//
// Created by xiao on 2022/4/12.
//
#include "OpenglRenderer.h"
#include "OpenglConstantBuffer.h"
#include "../../3rd/Glad/include/glad/glad.h"

using namespace asuna;


OpenglConstantBuffer::OpenglConstantBuffer(ConstantBufferDataType dt, int size) :
        ConstantBuffer(dt, size)
{
}

std::shared_ptr<OpenglConstantBuffer> OpenglConstantBuffer::Create(ConstantBufferDataType dt, int size)
{
    return std::make_shared<OpenglConstantBuffer>(dt, size);
}

void OpenglConstantBuffer::Bind()
{
    if (m_DataType == ConstantBufferDataType::PerFrame)
    {
        auto renderer = (OpenglRenderer*)Renderer::Current;
        renderer->SetConstantBufferDataPerFrame((ConstantBufferDataPerFrame*)this->GetData());
    }
    else if (m_DataType == ConstantBufferDataType::PerObject)
    {
        auto renderer = (OpenglRenderer*)Renderer::Current;
        renderer->SetConstantBufferDataPerObject((ConstantBufferDataPerObject*)this->GetData());
    }
}

void OpenglConstantBuffer::BindUniform(unsigned int program, const std::string& name, int offset, MaterialParameterType mpt)
{
    auto location = glGetUniformLocation(program, name.c_str());
    if (location == -1)
    {
        return;
    }
    if (mpt == MaterialParameterType::Vector4)
    {
        auto address = (char*)m_Data + offset;
        glUniform4fv(location, 1, (float*)address);
    }
    else if (mpt == MaterialParameterType::Matrix)
    {
        auto address = (char*)m_Data + offset;
        glUniformMatrix4fv(location, 1, true, (float*)address);
    }

}




