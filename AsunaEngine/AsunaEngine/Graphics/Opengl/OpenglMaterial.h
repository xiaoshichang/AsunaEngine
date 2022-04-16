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

    private:
        void BindPerFrameData(unsigned int program);
        void BindPerObjectData(unsigned int program);

    private:
        void InitProgram(unsigned int vs, unsigned int ps);
        unsigned int m_Program;

    };
}



