//
// Created by xiao on 2022/4/12.
//
#pragma once
#include <memory>
#include "../../3rd/Glad/include/glad/glad.h"
#include "../Abstract/Material.h"
#include "../Abstract/ConstantBuffer.h"

namespace asuna
{
    class OpenglConstantBuffer : public ConstantBuffer
    {
    public:
        OpenglConstantBuffer() = delete;
        explicit OpenglConstantBuffer(ConstantBufferDataType dt, int size);
        static std::shared_ptr<OpenglConstantBuffer> Create(ConstantBufferDataType dt, int size);

    public:
        void Bind() override;

    private:
        GLuint m_Buffer;

    };
}


