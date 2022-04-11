//
// Created by xiao on 2022/4/11.
//

#pragma once
#include <memory>
#include <d3d11.h>
#include "../Abstract/DepthStencilState.h"

namespace asuna
{
    class DirectX11DepthStencilState : public DepthStencilState
    {
    public:
        DirectX11DepthStencilState();
        ~DirectX11DepthStencilState() override;

    public:
        void Bind() override;

    private:
        void RebuildState();

    private:
        ID3D11DepthStencilState* m_State;

    };

}



