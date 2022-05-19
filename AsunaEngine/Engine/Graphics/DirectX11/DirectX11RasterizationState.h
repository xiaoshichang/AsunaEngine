#pragma once
#include <d3d11.h>
#include "../Abstract/RasterizationState.h"

namespace asuna
{

	class DirectX11RasterizationState : public RasterizationState
	{
	public:
		DirectX11RasterizationState(const RasterizationStateDesc& desc);
		~DirectX11RasterizationState() override;

	public:
		ID3D11RasterizerState* GetInternalState() { return m_State; }

	private:
		D3D11_RASTERIZER_DESC ConvertDesc(const RasterizationStateDesc& desc);

	private:
		ID3D11RasterizerState* m_State = nullptr;

	};

}