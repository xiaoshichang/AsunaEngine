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
		DirectX11RenderItem(std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> vs, std::shared_ptr<Shader> ps, std::shared_ptr<ConstantBuffer> cb):
			RenderItem(mesh, vs, ps, cb)
		{
		}

		virtual ~DirectX11RenderItem()
		{
		}


		virtual void Render() override;

		static std::shared_ptr<DirectX11RenderItem> Create(std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> vs, std::shared_ptr<Shader> ps, std::shared_ptr<ConstantBuffer> cb);
	};
}


