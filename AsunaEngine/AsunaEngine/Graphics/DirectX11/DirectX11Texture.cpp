#include "DirectX11Texture.h"
#include "../../Foundation/Platform/Assert.h"
using namespace asuna;


asuna::DirectX11Texture::~DirectX11Texture()
{
	if (m_Texture != nullptr)
	{
		m_Texture->Release();
		m_Texture = nullptr;
	}
}


std::shared_ptr<DirectX11Texture> asuna::DirectX11Texture::Create(int w, int h)
{
	return nullptr;
}
