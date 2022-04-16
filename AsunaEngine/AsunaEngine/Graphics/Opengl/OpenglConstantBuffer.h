//
// Created by xiao on 2022/4/12.
//
#pragma once
#include <memory>
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
        void BindUniform(unsigned int, const std::string& name, int offset, MaterialParameterType mpt);

    };
}


