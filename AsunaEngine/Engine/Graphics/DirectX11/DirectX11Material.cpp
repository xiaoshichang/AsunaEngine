//
// Created by xiao on 2022/4/18.
//

#include "DirectX11Material.h"
#include "DirectX11Renderer.h"
#include "DirectX11RenderContext.h"
#include "DirectX11Texture.h"

using namespace asuna;
using namespace std;

DirectX11Material::DirectX11Material(const string &path, MaterialType mt) : Material(path, mt)
{
}

DirectX11Material::~DirectX11Material() = default;

void asuna::DirectX11Material::Apply()
{
    m_VS->Bind();
    m_PS->Bind();
    m_PerMaterial->Bind();
    m_DepthStencilState->Bind();
    BindTextures();
}

void DirectX11Material::BindTextures()
{
    ID3D11SamplerState* state = nullptr;


    for(auto& pair : m_ParamDefines)
    {
        if (pair.second.m_Type == MaterialParameterType::Texture2D)
        {
            auto texture = GetTexture(pair.first);
            if (texture == nullptr)
            {
                continue;
            }
            auto mp = pair.second;
            if (texture->GetTextureType() == TextureType::ImageTexture)
            {
                auto imageTexture = dynamic_pointer_cast<DirectX11Texture>(texture);
                auto srv = imageTexture->GetSRV();
                BindTexture(mp.m_ShaderType, mp.m_Offset, state, srv);
            }
            else if(texture->GetTextureType() == TextureType::RTTexture)
            {
                auto rt = dynamic_pointer_cast<DirectX11RenderTarget>(texture);
                auto srv = rt->GetSRV();
                BindTexture(mp.m_ShaderType, mp.m_Offset, state, srv);
            }
            else
            {
                ASUNA_ASSERT(false);
            }
        }
    }
}

void DirectX11Material::BindTexture(TextureShaderType tst, int offset, ID3D11SamplerState* state, ID3D11ShaderResourceView* srv)
{
    auto context = dynamic_pointer_cast<DirectX11RenderContext>(Renderer::Instance->GetContext());

    if (tst == TextureShaderType::VS)
    {
        context->m_DeviceContext->VSSetShaderResources(offset, 1, &srv);
        context->m_DeviceContext->VSSetSamplers(offset, 1, &state);
    }
    else if (tst == TextureShaderType::PS)
    {
        context->m_DeviceContext->PSSetShaderResources(offset, 1, &srv);
        context->m_DeviceContext->PSSetSamplers(offset, 1, &state);
    }
    else
    {
        ASUNA_ASSERT(false);
    }
}


