//
// Created by xiao on 2022/4/12.
//
#pragma once
#include "../Abstract/RenderItemQueue.h"


namespace asuna
{
    class OpenglRenderItemQueue : public RenderItemQueue
    {
    private:
        void BindConstantBufferPerFrame() override;
    };
}

