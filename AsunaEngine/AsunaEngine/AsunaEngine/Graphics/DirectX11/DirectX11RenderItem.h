#pragma once
#include "../RenderItem.h"
#include "DirectX11IndexBuffer.h"
#include "DirectX11VertexBuffer.h"
#include "DirectX11Shader.h"


namespace asuna
{
	class DirectX11RenderItem : public RenderItem
	{
	public:
		DirectX11RenderItem(DirectX11VertexBuffer* vb, DirectX11IndexBuffer* ib, DirectX11VextexShader* vs, DirectX11PixelShader* ps) :
			m_VertexBuffer(vb),
			m_IndexBuffer(ib),
			m_VertexShader(vs),
			m_PixelShader(ps)
		{
		}

		virtual void Render() override;

	private:
		DirectX11VertexBuffer* m_VertexBuffer;
		DirectX11IndexBuffer* m_IndexBuffer;
		DirectX11VextexShader* m_VertexShader;
		DirectX11PixelShader* m_PixelShader;
	};
}


