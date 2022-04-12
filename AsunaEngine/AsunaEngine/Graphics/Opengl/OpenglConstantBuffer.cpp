//
// Created by xiao on 2022/4/12.
//
#include "OpenGLRenderer.h"
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
        auto renderer = (OpenGLRenderer*)Renderer::Current;
        renderer->SetConstantBufferDataPerFrame((ConstantBufferDataPerFrame*)this->GetData());
    }
    else if (m_DataType == ConstantBufferDataType::PerObject)
    {
        auto renderer = (OpenGLRenderer*)Renderer::Current;
        renderer->SetConstantBufferDataPerObject((ConstantBufferDataPerObject*)this->GetData());
    }
}

void OpenglConstantBuffer::BindPerFrameData(unsigned int program)
{
    auto renderer = (OpenGLRenderer*)Renderer::Current;
    auto data = renderer->GetConstantBufferDataPerFrame();
    // Set the view matrix in the vertex shader.
    auto location = glGetUniformLocation(program, "viewMatrix");
    if (location == -1)
    {
        return;
    }
    // transpose matrix by setting 3rd parameter to true
    glUniformMatrix4fv(location, 1, true, data->m_ViewMatrix);

    // Set the projection matrix in the vertex shader.
    location = glGetUniformLocation(program, "projectionMatrix");
    if (location == -1)
    {
        return;
    }
    glUniformMatrix4fv(location, 1, true, data->m_ProjectionMatrix);
}

void OpenglConstantBuffer::BindPerObjectData(unsigned int program)
{
    auto renderer = (OpenGLRenderer*)Renderer::Current;
    auto data = renderer->GetConstantBufferDataPerObject();
    // Set the world matrix in the vertex shader.
    auto location = glGetUniformLocation(program, "worldMatrix");
    if (location == -1)
    {
        return;
    }
    glUniformMatrix4fv(location, 1, true, data->m_WorldMatrix);
}

void OpenglConstantBuffer::BindPerMaterialData(unsigned int program)
{
    auto location = glGetUniformLocation(program, "BaseColor");
    if (location == -1)
    {
        return;
    }
    glUniform4fv(location, 1, (float*)m_Data);
}

void OpenglConstantBuffer::Bind(unsigned int program)
{
    BindPerFrameData(program);
    BindPerObjectData(program);
    BindPerMaterialData(program);
}




