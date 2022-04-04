//
// Created by xiao on 2022/4/3.
//

#pragma once
#include <memory>
#include <string>
#include "../../../Graphics/Mesh.h"
#include "../../../Graphics/Shader.h"
#include "../../../Graphics/RenderItem.h"
#include "../Component.h"


namespace asuna
{
    class MeshRenderCmpt : public Component
    {
    public:
        MeshRenderCmpt() = delete;
        explicit MeshRenderCmpt(GameObject* owner) :
                m_MeshPath(),
                m_VSPath(),
                m_PSPath(),
                m_RenderItem(nullptr),
                Component(owner)
        {
        }

        ~MeshRenderCmpt() override
        {
        }

        void Initialize() override;
        void Finalize() override;
        void Tick() override;


    public:
        void SetMesh(const std::string& path);
        void SetMaterial(const std::string& vs, const std::string& ps);

        const std::string& GetMeshPath()
        {
            return m_MeshPath;
        }

        const std::string& GetVSPath()
        {
            return m_VSPath;
        }

        const std::string& GetPSPath()
        {
            return m_PSPath;
        }

    private:
        std::string m_MeshPath;
        std::string m_VSPath;
        std::string m_PSPath;
        std::shared_ptr<RenderItem> m_RenderItem;
    };
}