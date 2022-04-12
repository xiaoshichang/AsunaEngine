//
// Created by xiao on 2022/4/12.
//

#pragma once
#include "../Abstract/DepthStencilState.h"

namespace asuna
{
    class OpenglDepthStencilState : public DepthStencilState
    {
    public:
        OpenglDepthStencilState();
        ~OpenglDepthStencilState() override;
    public:
        void Bind() override;
    };
}



