//
// Created by xiao on 2022/4/12.
//
#pragma once
#include <memory>
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
        void Bind(unsigned int program);

    private:
        void BindPerFrameData(unsigned int program);
        void BindPerObjectData(unsigned int program);
        void BindPerMaterialData(unsigned int program);

    };
}


