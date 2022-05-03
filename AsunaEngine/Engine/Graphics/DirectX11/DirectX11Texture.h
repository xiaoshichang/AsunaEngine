#pragma once
#include <memory>
#include <d3d11.h>
#include "../Abstract/Texture.h"

namespace asuna
{
	class DirectX11Texture : public Texture
	{
	public:
		DirectX11Texture() = delete;
		DirectX11Texture(const std::shared_ptr<RawTexture> &rawImage, ID3D11Texture2D *texture, ID3D11ShaderResourceView* srv) :
                Texture(rawImage),
                m_Texture(texture),
                m_SRV(srv)
		{
		}

		~DirectX11Texture() override;

		ID3D11Texture2D* GetTexture()
		{
			return m_Texture;
		}

        ID3D11ShaderResourceView* GetSRV()
        {
            return m_SRV;
        }


		static std::shared_ptr<DirectX11Texture> Create(const std::shared_ptr<RawTexture>& rawTexture);

	private:
		ID3D11Texture2D* m_Texture;
        ID3D11ShaderResourceView* m_SRV;
	};


}