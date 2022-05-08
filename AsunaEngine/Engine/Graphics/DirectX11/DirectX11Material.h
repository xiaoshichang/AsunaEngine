//
// Created by xiao on 2022/4/18.
//

#pragma once


#include <d3d11.h>
#include "../Abstract/Material.h"


namespace asuna
{
    class DirectX11Material : public Material
    {

    public:
        explicit DirectX11Material(const std::string &path, MaterialType mt);
        ~DirectX11Material() override;
        void Apply() override;

    protected:
        void BindTextures() override;
        void BindTexture(TextureShaderType tst, int offset, ID3D11SamplerState* state, ID3D11ShaderResourceView* srv);

    };

}

