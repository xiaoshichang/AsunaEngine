//
// Created by xiao on 2022/4/5.
//

#pragma once
#include "../RenderItemQueue.h"
#include "../RenderContext.h"

namespace asuna
{
    class DirectX11RenderItemQueue : public RenderItemQueue
    {
    private:
        void BindConstantBufferPerFrame() override;
    };
}



