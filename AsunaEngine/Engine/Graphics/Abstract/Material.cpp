//
// Created by xiao on 2022/4/9.
//

#include "Material.h"

#include <utility>
#include "Renderer.h"

using namespace asuna;
using namespace std;

Material::Material(const std::string& materialName, MaterialType mt) :
    m_MaterialType(mt),
    m_MaterialName(materialName)
{
    if (Renderer::Instance->m_APIType == RenderAPIType::Directx11)
    {
        string generated_dir(R"(Assets\Shaders\Generated\dx11\)");
        string target = generated_dir + materialName + "_VS.hlsl";
        m_VS = Renderer::Instance->CreateShader(target, ShaderType::VertexShader);
        target = generated_dir + materialName + "_PS.hlsl";
        m_PS = Renderer::Instance->CreateShader(target, ShaderType::PixelShader);
    }
    else
    {
        string generated_dir(R"(Assets\Shaders\Generated\opengl\)");
        string target = generated_dir + materialName + "_VS.glsl";
        m_VS = Renderer::Instance->CreateShader(target, ShaderType::VertexShader);
        target = generated_dir + materialName + "_PS.glsl";
        m_PS = Renderer::Instance->CreateShader(target, ShaderType::PixelShader);
    }

    m_PerMaterial = Renderer::Instance->CreateConstantBuffer(ConstantBufferDataType::PerMaterial, 1024);
    m_DepthStencilState = Renderer::Instance->CreateDepthStencilState();
    BuildMaterialParametersLayout();
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
    if (!CheckParameterExist(name))
    {
        return;
    }

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
    if (!CheckParameterExist(name))
    {
        return;
    }

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

void Material::SetMatrix(const string &name, Matrix4x4f value)
{
    if (!CheckParameterExist(name))
    {
        return;
    }

    auto pair = m_MatrixParams.find(name);
    if (pair == m_MatrixParams.end())
    {
        m_MatrixParams.insert({name, value});
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
        memcpy(address, &value, sizeof(Matrix4x4f));
    }
}

Matrix4x4f Material::GetMatrix(const string &name)
{
    auto pair = m_MatrixParams.find(name);
    if (pair == m_MatrixParams.end())
    {
        return {};
    }
    else
    {
        return pair->second;
    }
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

bool Material::CheckParameterExist(const std::string &name) const
{
    if (m_ParamDefines.find(name) == m_ParamDefines.end())
    {
        return false;
    }
    return true;
}

void Material::BuildMaterialParametersLayout()
{
    m_ParamDefines["BaseColor"] = {MaterialParameterType::Vector4, 0, TextureShaderType::PS};
    m_ParamDefines["ModelMatrix"] = {MaterialParameterType::Matrix, 16, TextureShaderType::VS};

    m_ParamDefines["MainTex"] = {MaterialParameterType::Texture2D, 0, TextureShaderType::PS};
}

void Material::SetTexture(const string &name, const shared_ptr<Texture> &value)
{
    if (!CheckParameterExist(name))
    {
        return;
    }

    auto kv = m_TextureParams.find(name);
    if (kv != m_TextureParams.end())
    {
        kv->second = value;
    }
    else
    {
        m_TextureParams.insert({name, value});
    }
}

std::shared_ptr<Texture> Material::GetTexture(const string &name)
{
    auto kv = m_TextureParams.find(name);
    if (kv != m_TextureParams.end())
    {
        return kv->second;
    }
    return nullptr;
}

const MaterialType Material::GetMaterialType()
{
    return m_MaterialType;
}




