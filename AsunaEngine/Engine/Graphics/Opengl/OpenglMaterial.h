//
// Created by xiao on 2022/4/12.
//

#pragma once
#include <memory>
#include "../Abstract/Material.h"

namespace asuna
{

    class OpenglMaterial : public Material
    {

    public:
        explicit OpenglMaterial(const std::string& path);
        ~OpenglMaterial() override;
        static std::shared_ptr<OpenglMaterial> Create(const std::string& path);
        void Apply() override;

    protected:
        void BindTextures() override;

    private:
        void InitProgram(unsigned int vs, unsigned int ps);
        unsigned int m_Program;

    };
}


