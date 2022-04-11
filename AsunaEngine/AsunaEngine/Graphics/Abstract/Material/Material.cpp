//
// Created by xiao on 2022/4/9.
//

#include "Material.h"
#include "../Renderer.h"

using namespace asuna;
using namespace std;

Material::Material(const std::string& materialPath) :
    m_MaterialName(materialPath)
{
    m_VS = Renderer::Current->CreateShader("Assets\\Shaders\\triangle.vs", ShaderType::VertexShader);
    m_PS = Renderer::Current->CreateShader("Assets\\Shaders\\triangle.ps", ShaderType::PixelShader);
    m_PerMaterial = Renderer::Current->CreateConstantBuffer(ConstantBufferDataType::PerMaterial, 512);
    m_DepthStencilState = Renderer::Current->CreateDepthStencilState();
    BuildMaterialParametersLayout();
}

shared_ptr<Material> Material::Create(const std::string& materialPath)
{
    return make_shared<Material>(materialPath);
}

void Material::Apply()
{
    m_VS->Bind();
    m_PS->Bind();
    m_PerMaterial->Bind();
    m_DepthStencilState->Bind();
}

float Material::GetFloat(const string &name)
{
    auto pair = m_FloatParams.find(name);
    if (pair == m_FloatParams.end())
    {
        return 0;
    }
    else
    {
        return pair->second;
    }
}

void Material::SetFloat(const string &name, float value)
{
    auto pair = m_FloatParams.find(name);
    if (pair == m_FloatParams.end())
    {
        m_FloatParams.insert({name, value});
    }
    else
    {
        pair->second = value;
    }

    auto offset = GetParamOffset(name);
    if (offset == -1)
    {
        Logger::Warning("%s not found in parameter defines", name.c_str());
    }
    else
    {
        char* buffer = (char*)m_PerMaterial->GetData();
        char* address = buffer + offset;
        memcpy(address, &value, sizeof(float));
    }

}

void Material::SetVector4(const string &name, Vector4f value)
{
    auto pair = m_Vector4Params.find(name);
    if (pair == m_Vector4Params.end())
    {
        m_Vector4Params.insert({name, value});
    }
    else
    {
        pair->second = value;
    }

    auto offset = GetParamOffset(name);
    if (offset == -1)
    {
        Logger::Warning("%s not found in parameter defines", name.c_str());
    }
    else
    {
        char* buffer = (char*)m_PerMaterial->GetData();
        char* address = buffer + offset;
        memcpy(address, &value, sizeof(Vector4f));
    }
}

Vector4f Material::GetVector4(const string &name)
{
    auto pair = m_Vector4Params.find(name);
    if (pair == m_Vector4Params.end())
    {
        return {0, 0, 0, 0};
    }
    return m_Vector4Params[name];
}

const string& Material::GetName()
{
    return m_MaterialName;
}

int Material::GetParamOffset(const string &name)
{
    auto kv = m_ParamDefines.find(name);
    if (kv != m_ParamDefines.end())
    {
        return kv->second.m_Offset;
    }
    return -1;
}

MaterialParameterType Material::GetParamType(const string &name)
{
    auto kv = m_ParamDefines.find(name);
    if (kv != m_ParamDefines.end())
    {
        return kv->second.m_Type;
    }
    return MaterialParameterType::Unknown;
}

void Material::BuildMaterialParametersLayout()
{
    m_ParamDefines["BaseColor"] = {0, MaterialParameterType::Vector4};
}
