//
// Created by xiao on 2022/4/12.
//
#include "OpenglRenderer.h"
#include "OpenglMaterial.h"
#include "OpenglShader.h"
#include "OpenglConstantBuffer.h"
#include "OpenglTexture.h"
#include "OpenglRenderTarget.h"

#include "../../3rd/Glad/include/glad/glad.h"
#include "../../Foundation/Logger/Logger.h"

using namespace asuna;
using namespace std;

OpenglMaterial::OpenglMaterial(const string &path, MaterialType mt) :
        m_Program(0),
        Material(path, mt)
{
    auto vs = dynamic_pointer_cast<OpenglShader>(m_VS);
    auto ps = dynamic_pointer_cast<OpenglShader>(m_PS);
    InitProgram(vs->GetShader(), ps->GetShader());
}

OpenglMaterial::~OpenglMaterial() = default;

void OpenglMaterial::Apply()
{
    glUseProgram(m_Program);
    m_PerMaterial->Bind();
    m_DepthStencilState->Bind();
    BindTextures();
}

void OpenglMaterial::InitProgram(unsigned int vs, unsigned int ps)
{
    // Create a shader program object.
    m_Program = glCreateProgram();
    // Attach the vertex and fragment shader to the program object.
    glAttachShader(m_Program, vs);
    glAttachShader(m_Program, ps);
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

void OpenglMaterial::BindTextures()
{
    for(auto& pair : m_ParamDefines)
    {
        if (pair.second.m_Type == MaterialParameterType::Texture2D)
        {
            auto texture = GetTexture(pair.first);
            if (texture == nullptr)
            {
                continue;
            }
            if (texture->GetTextureType() == TextureType::ImageTexture)
            {
                auto imageTexture = dynamic_pointer_cast<OpenglTexture>(texture);
                auto loc = glGetUniformLocation(m_Program, pair.first.c_str());
                glUniform1i(loc, pair.second.m_Offset);
                glActiveTexture(GL_TEXTURE0 + pair.second.m_Offset);
                glBindTexture(GL_TEXTURE_2D, imageTexture->GetTexture());
            }
            else
            {
                auto rt = dynamic_pointer_cast<OpenglRenderTarget>(texture);
                auto loc = glGetUniformLocation(m_Program, pair.first.c_str());
                glUniform1i(loc, pair.second.m_Offset);
                glActiveTexture(GL_TEXTURE0 + pair.second.m_Offset);
                glBindTexture(GL_TEXTURE_2D, rt->GetTexture());
            }
        }
    }
}



