#pragma once
#include <memory>
#include "../Abstract/RenderItem.h"
#include "DirectX11Mesh.h"
#include "DirectX11Shader.h"
#include "DirectX11ConstantBuffer.h"
#include "DirectX11RenderContext.h"


namespace asuna
{
	class DirectX11RenderItem : public RenderItem
	{
	public:
		DirectX11RenderItem(const std::shared_ptr<Mesh>& mesh,
                            const std::vector<std::shared_ptr<Material>>& materials,
                            const std::shared_ptr<ConstantBuffer>& perObject):
			RenderItem(mesh, materials, perObject)
		{
		}

        DirectX11RenderItem(const std::shared_ptr<Mesh>& mesh,
                            const std::shared_ptr<ConstantBuffer>& perObject):
                RenderItem(mesh, perObject)
        {
        }

		~DirectX11RenderItem() override = default;

		void Render() const override;
		static std::shared_ptr<DirectX11RenderItem> Create(
                const std::shared_ptr<Mesh>& mesh,
                const std::vector<std::shared_ptr<Material>>& materials,
                const std::shared_ptr<ConstantBuffer>& perObject);

        static std::shared_ptr<DirectX11RenderItem> Create(
                const std::shared_ptr<Mesh>& mesh,
                const std::shared_ptr<ConstantBuffer>& perObject);

    private:
        void BindConstantBufferPerObject(DirectX11RenderContext* context) const;
        void DrawMesh(DirectX11RenderContext* context) const;
	};
}


