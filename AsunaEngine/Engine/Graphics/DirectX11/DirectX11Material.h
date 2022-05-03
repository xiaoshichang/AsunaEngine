//
// Created by xiao on 2022/4/18.
//

#pragma once
#include "../Abstract/Material.h"

namespace asuna
{
    class DirectX11Material : public Material
    {

    public:
        DirectX11Material(const std::string& path);
        ~DirectX11Material() override;
        void Apply() override;
        static std::shared_ptr<DirectX11Material> Create(const std::string& path);

    protected:
        void BindTextures() override;

    };

}

