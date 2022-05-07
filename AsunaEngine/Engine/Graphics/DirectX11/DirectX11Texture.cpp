#include "DirectX11Texture.h"
#include "DirectX11RenderContext.h"
#include "../Abstract/Renderer.h"
#include "../../Foundation/Platform/Assert.h"

using namespace asuna;
using namespace std;


asuna::DirectX11Texture::~DirectX11Texture()
{
	if (m_Texture != nullptr)
	{
		m_Texture->Release();
		m_Texture = nullptr;
	}
    if (m_SRV != nullptr)
    {
        m_SRV->Release();
        m_SRV = nullptr;
    }
}

std::shared_ptr<DirectX11Texture> DirectX11Texture::Create(const std::shared_ptr<RawTexture> &rawTexture)
{
    ID3D11Texture2D* texture;
    ID3D11ShaderResourceView* srv;

    D3D11_SUBRESOURCE_DATA initData;
    initData.pSysMem = rawTexture->GetData();
    initData.SysMemPitch = rawTexture->GetWidth() * rawTexture->GetPixelBytes();
    initData.SysMemSlicePitch = 0;

    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = rawTexture->GetWidth();
    desc.Height = rawTexture->GetHeight();
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_IMMUTABLE;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    auto context = dynamic_pointer_cast<DirectX11RenderContext>(Renderer::Instance->GetContext());
    auto hr = context->m_Device->CreateTexture2D( &desc, &initData, &texture);
    ASUNA_ASSERT(SUCCEEDED(hr));

    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
    SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MipLevels = 1;

    hr = context->m_Device->CreateShaderResourceView(texture, &SRVDesc, &srv);
    ASUNA_ASSERT(SUCCEEDED(hr));

    return make_shared<DirectX11Texture>(rawTexture, texture, srv);
}
