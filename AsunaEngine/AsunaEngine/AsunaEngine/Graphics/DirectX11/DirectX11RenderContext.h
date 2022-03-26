#pragma once
#include <d3d11.h>
#include "../RenderContext.h"
namespace asuna
{
	class DirectX11RenderContext : public RenderContext
	{
	public:
		DirectX11RenderContext() :
			m_Device(nullptr),
			m_DeviceContext(nullptr)
		{
		}

		virtual ~DirectX11RenderContext()
		{
		}

	public:
		ID3D11Device* m_Device;
		ID3D11DeviceContext* m_DeviceContext;
	};
}


