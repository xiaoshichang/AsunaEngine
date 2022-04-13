//
// Created by xiao on 2022/4/12.
//

#include "OpenglMaterial.h"
#include "OpenglShader.h"
#include "OpenglConstantBuffer.h"
#include "../../3rd/Glad/include/glad/glad.h"
#include "../../Foundation/Logger/Logger.h"

using namespace asuna;

OpenglMaterial::OpenglMaterial(const std::string &path) :
        m_Program(0),
        Material(path)
{
    auto vs = std::dynamic_pointer_cast<OpenglShader>(m_VS);
    auto ps = std::dynamic_pointer_cast<OpenglShader>(m_PS);
    InitProgram(vs->GetShader(), ps->GetShader());
}

OpenglMaterial::~OpenglMaterial() = default;

std::shared_ptr<OpenglMaterial> OpenglMaterial::Create(const std::string &path)
{
    return std::make_shared<OpenglMaterial>(path);
}

void OpenglMaterial::Apply()
{
    glUseProgram(m_Program);
    auto openglMaterial = std::dynamic_pointer_cast<OpenglConstantBuffer>(m_PerMaterial);
    openglMaterial->Bind(m_Program);
    m_DepthStencilState->Bind();
}

void OpenglMaterial::InitProgram(unsigned int vs, unsigned int ps)
{
    // Create a shader program object.
    m_Program = glCreateProgram();
    // Attach the vertex and fragment shader to the program object.
    glAttachShader(m_Program, vs);
    glAttachShader(m_Program, ps);
    // Bind the shader input variables.
    glBindAttribLocation(m_Program, 0, "inputPosition");
    glBindAttribLocation(m_Program, 1, "inputColor");
    // Link the shader program.
    glLinkProgram(m_Program);
    // Check the status of the link.
    int status;
    glGetProgramiv(m_Program, GL_LINK_STATUS, &status);
    if (status != 1)
    {
        //Retrieve compiler error messages when linking fails
        GLint infolen = 0;
        glGetProgramiv(m_Program, GL_INFO_LOG_LENGTH, &infolen);
        if (infolen > 1)
        {
            char* infoLog = new char[infolen + 1];
            glGetProgramInfoLog(m_Program, infolen, nullptr, infoLog);
            Logger::Error("Error linking program:%s", infoLog);
            free(infoLog);
        }
        ASUNA_ASSERT(false);
    }
}


