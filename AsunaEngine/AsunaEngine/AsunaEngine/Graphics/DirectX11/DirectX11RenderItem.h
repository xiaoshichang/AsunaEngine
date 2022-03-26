#pragma once
#include <memory>
#include "../RenderItem.h"
#include "DirectX11Mesh.h"
#include "DirectX11Shader.h"
#include "DirectX11ConstantBuffer.h"


namespace asuna
{
	class DirectX11RenderItem : public RenderItem
	{
	public:
		DirectX11RenderItem(std::shared_ptr<DirectX11Mesh> mesh,
			std::shared_ptr<DirectX11VextexShader> vs,
			std::shared_ptr<DirectX11PixelShader> ps,
			std::shared_ptr<DirectX11ConstantBuffer> constant
		) :
			m_Mesh(mesh),
			m_VertexShader(vs),
			m_PixelShader(ps),
			m_ConstantBuffer(constant)
		{
		}

		virtual ~DirectX11RenderItem()
		{
		}


		virtual void Render() override;

	private:
		std::shared_ptr<DirectX11Mesh> m_Mesh;
		std::shared_ptr<DirectX11VextexShader> m_VertexShader;
		std::shared_ptr<DirectX11PixelShader> m_PixelShader;
		std::shared_ptr<DirectX11ConstantBuffer> m_ConstantBuffer;
	};
}


