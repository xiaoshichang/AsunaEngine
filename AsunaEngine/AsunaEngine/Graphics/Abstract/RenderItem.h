#pragma once
#include <memory>
#include <utility>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Mesh.h"
#include "Shader.h"
#include "RenderItem.h"
#include "ConstantBuffer.h"
#include "Material/Material.h"


namespace asuna
{
	class RenderItem
	{
	public:
		RenderItem() = delete;
		RenderItem(
                const std::shared_ptr<Mesh>& mesh,
                std::vector<std::shared_ptr<Material>>  materials,
                const std::shared_ptr<ConstantBuffer>& perObject)
		{
            m_Mesh = mesh;
            m_ConstantBufferPerObject = perObject;
            m_Materials = std::move(materials);
        }

        RenderItem(
                const std::shared_ptr<Mesh>& mesh,
                const std::shared_ptr<ConstantBuffer>& perObject)
        {
            m_Mesh = mesh;
            m_ConstantBufferPerObject = perObject;
        }

		virtual ~RenderItem() = default;

        std::shared_ptr<Mesh> GetMesh()
		{
			return m_Mesh;
		}

        void SetMesh(const std::shared_ptr<Mesh>& mesh)
        {
            m_Mesh = mesh;
        }

        std::shared_ptr<ConstantBuffer> GetConstantBufferPerObject()
		{
			return m_ConstantBufferPerObject;
		}

        void SetConstantBuffer(const std::shared_ptr<ConstantBuffer>& cb)
        {
            m_ConstantBufferPerObject = cb;
        }

        void AllocateMaterials(int count);
        void SetMaterial(int index, const std::shared_ptr<Material>& material);
        std::shared_ptr<Material> GetMaterial(int index) const;
        int GetMaterialCount() const;

	public:
		virtual void Render() = 0;


	private:
        std::shared_ptr<Mesh> m_Mesh;
        std::vector<std::shared_ptr<Material>> m_Materials;
        std::shared_ptr<ConstantBuffer> m_ConstantBufferPerObject;
	};
}


