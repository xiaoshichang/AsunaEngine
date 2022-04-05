#pragma once
#include <memory>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Mesh.h"
#include "Shader.h"
#include "RenderItem.h"
#include "ConstantBuffer.h"

namespace asuna
{
	class RenderItem
	{
	public:
		RenderItem() = delete;
		RenderItem(
			std::shared_ptr<Mesh>& mesh,
			std::shared_ptr<Shader>& vertexShader,
			std::shared_ptr<Shader>& pixelShader,
			std::shared_ptr<ConstantBuffer>& perObject,
            std::shared_ptr<ConstantBuffer>& perScene
            ):
			m_Mesh(mesh),
			m_VertexShader(vertexShader),
			m_PixelShader(pixelShader),
			m_ConstantBufferPerObject(perObject),
            m_ConstantBufferPerScene(perScene)
		{}

		virtual ~RenderItem() = default;

		std::shared_ptr<Mesh> GetMesh()
		{
			return m_Mesh;
		}

        void SetMesh(const std::shared_ptr<Mesh>& mesh)
        {
            m_Mesh = mesh;
        }

		std::shared_ptr<Shader> GetVertexShader()
		{
			return m_VertexShader;
		}

        void SetVertexShader(const std::shared_ptr<Shader>& vs)
        {
            m_VertexShader = vs;
        }

		std::shared_ptr<Shader> GetPixelShader()
		{
			return m_PixelShader;
		}

        void SetPixelShader(const std::shared_ptr<Shader>& ps)
        {
            m_PixelShader = ps;
        }

		std::shared_ptr<ConstantBuffer> GetConstantBufferPerObject()
		{
			return m_ConstantBufferPerObject;
		}

        void SetConstantBuffer(const std::shared_ptr<ConstantBuffer>& cb)
        {
            m_ConstantBufferPerObject = cb;
        }

        std::shared_ptr<ConstantBuffer> GetConstantBufferPerScene()
        {
            return m_ConstantBufferPerScene;
        }

        void SetConstantBufferPerScene(const std::shared_ptr<ConstantBuffer>& cb)
        {
            m_ConstantBufferPerScene = cb;
        }


	public:
		virtual void Render() = 0;


	private:
		std::shared_ptr<Mesh> m_Mesh;
		std::shared_ptr<Shader> m_VertexShader;
		std::shared_ptr<Shader> m_PixelShader;
		std::shared_ptr<ConstantBuffer> m_ConstantBufferPerObject;
        std::shared_ptr<ConstantBuffer> m_ConstantBufferPerScene;
	};
}


