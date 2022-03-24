#pragma once
#include "../RenderItem.h"
#include "DirectX11Mesh.h"
#include "DirectX11Shader.h"
#include "DirectX11ConstantBuffer.h"


namespace asuna
{
	class DirectX11RenderItem : public RenderItem
	{
	public:
		DirectX11RenderItem(DirectX11Mesh* mesh, 
			DirectX11VextexShader* vs, 
			DirectX11PixelShader* ps, 
			DirectX11ConstantBuffer* constant
		) :
			m_Mesh(mesh),
			m_VertexShader(vs),
			m_PixelShader(ps),
			m_ConstantBuffer(constant)
		{
		}

		virtual void Render() override;

	private:
		DirectX11Mesh* m_Mesh;
		DirectX11VextexShader* m_VertexShader;
		DirectX11PixelShader* m_PixelShader;
		DirectX11ConstantBuffer* m_ConstantBuffer;
	};
}


