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

		~DirectX11RenderItem() override = default;

		virtual void Render() override;

		static std::shared_ptr<DirectX11RenderItem> Create(
                const std::shared_ptr<Mesh>& mesh,
                const std::shared_ptr<Shader>& vs,
                const std::shared_ptr<Shader>& ps,
                const std::shared_ptr<ConstantBuffer>& cb);
	};
}


