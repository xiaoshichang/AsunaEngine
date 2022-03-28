#pragma once
#include <memory>
#include <d3d11.h>
#include "../Texture.h"

namespace asuna
{
	class DirectX11Texture : public Texture
	{
	public:
		DirectX11Texture() = delete;
		DirectX11Texture(ID3D11Texture2D* texture):
			m_Texture(texture)
		{
		}

		virtual ~DirectX11Texture();

		ID3D11Texture2D* GetTexture()
		{
			return m_Texture;
		}

		static std::shared_ptr<DirectX11Texture> Create(int w, int h);

	private:
		ID3D11Texture2D* m_Texture;
	};


}