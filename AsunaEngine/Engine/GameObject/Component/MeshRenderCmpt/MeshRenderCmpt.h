//
// Created by xiao on 2022/4/3.
//

#pragma once
#include <memory>
#include <string>
#include "../../../Graphics/Abstract/Mesh.h"
#include "../../../Graphics/Abstract/Shader.h"
#include "../../../Graphics/Abstract/RenderItem.h"
#include "../Component.h"


namespace asuna
{
    class MeshRenderCmpt : public Component
    {
    public:
        MeshRenderCmpt() = delete;
        explicit MeshRenderCmpt(GameObject* owner) : Component(owner)
        {
        }

        ~MeshRenderCmpt() override = default;

        void Initialize() override;
        void Finalize() override;
        void Tick() override;


    public:
        const RenderItem* GetRenderItem();
        void SetMesh(const std::shared_ptr<Mesh>& mesh);
        void SetMaterial(int index, const std::shared_ptr<Material>& material);
        int GetMaterialCount() const;
        std::shared_ptr<Material> GetMaterial(int index);
        bool& GetCastShadowFlag() {return m_CastShadow;}
        void SetCastShadowFlag(bool flag) { m_CastShadow = flag;}

    private:
        void UpdateConstantBufferPerObject();

    private:
        std::shared_ptr<RenderItem> m_RenderItem = nullptr;
        std::shared_ptr<ConstantBuffer> m_ConstantBufferPerObject = nullptr;
        bool m_CastShadow = true;
    };
}
