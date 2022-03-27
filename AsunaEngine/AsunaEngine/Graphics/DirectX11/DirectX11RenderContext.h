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
			if (m_Device != nullptr)
			{
				m_Device->Release();
				m_Device = nullptr;
			}
			if (m_DeviceContext != nullptr)
			{
				m_DeviceContext->Release();
				m_DeviceContext = nullptr;
			}
		}

	public:
		ID3D11Device* m_Device;
		ID3D11DeviceContext* m_DeviceContext;
	};
}


