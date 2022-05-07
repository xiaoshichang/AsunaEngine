//
// Created by xiao on 2022/4/18.
//

#include "DirectX11Material.h"
#include "DirectX11Renderer.h"
#include "DirectX11RenderContext.h"
#include "DirectX11Texture.h"

using namespace asuna;
using namespace std;

DirectX11Material::DirectX11Material(const string &path) : Material(path)
{
}

DirectX11Material::~DirectX11Material()
{
}

void asuna::DirectX11Material::Apply()
{
    m_VS->Bind();
    m_PS->Bind();
    m_PerMaterial->Bind();
    m_DepthStencilState->Bind();
    BindTextures();
}

shared_ptr<DirectX11Material> DirectX11Material::Create(const string &path)
{
    return make_shared<DirectX11Material>(path);
}

void DirectX11Material::BindTextures()
{
    ID3D11SamplerState* state = nullptr;


    for(auto& pair : m_ParamDefines)
    {
        if (pair.second.m_Type == MaterialParameterType::Texture2D)
        {
            auto texture = dynamic_pointer_cast<DirectX11Texture>(GetTexture(pair.first));
            if (texture == nullptr)
            {
                continue;
            }

            auto context = dynamic_pointer_cast<DirectX11RenderContext>(Renderer::Instance->GetContext());
            auto mp = pair.second;
            if (mp.m_ShaderType == TextureShaderType::VS)
            {
                auto srv = texture->GetSRV();
                context->m_DeviceContext->VSSetShaderResources(mp.m_Offset, 1, &srv);
                context->m_DeviceContext->VSSetSamplers(mp.m_Offset, 1, &state);
            }
            else if (mp.m_ShaderType == TextureShaderType::PS)
            {
                auto srv = texture->GetSRV();
                context->m_DeviceContext->PSSetShaderResources(mp.m_Offset, 1, &srv);
                context->m_DeviceContext->PSSetSamplers( mp.m_Offset, 1, &state);
            }
            else
            {
                ASUNA_ASSERT(false);
            }
        }
    }


}


