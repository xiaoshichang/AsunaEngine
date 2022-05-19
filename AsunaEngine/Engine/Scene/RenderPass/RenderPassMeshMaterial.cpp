//
// Created by xiao on 2022/5/7.
//

#include "RenderPassMeshMaterial.h"
#include "../SceneManager.h"
#include "../../Graphics/Abstract/Renderer.h"
#include "../../GameObject/Component/MeshRenderCmpt/MeshRenderCmpt.h"

using namespace asuna;
using namespace std;

RenderPassMeshMaterial::RenderPassMeshMaterial(CreateRendererContextParam param)
{
    RenderTargetDesc desc{};
    desc.usage = RenderTargetUsage::Default;
    desc.width = param.m_ResolutionWith;
    desc.height = param.m_ResolutionHeight;
    m_MainRT = Renderer::Instance->CreateRenderTarget(desc);
    CreateCoordAxisRenderItem();
}

void RenderPassMeshMaterial::Render(const std::shared_ptr<RasterizationState>& rs)
{
    SceneManager::Instance->GetConstantBufferPerScene()->Bind();
    Renderer::Instance->SetRenderTarget(m_MainRT);
    Renderer::Instance->ClearRenderTarget(m_MainRT, 0.1f, 0.2f, 0.3f, 1.0f);
    Renderer::Instance->SetRasterizationState(rs);
	Renderer::Instance->SetViewPort(0, 0, m_MainRT->GetWidth(), m_MainRT->GetHeight());

    CollectRenderItems();
    for(auto item : m_Items)
    {
        item->Render();
    }

    if (m_ShowCoordAxis)
    {
        for(const auto& item : m_AxisRenderItems)
        {
            item->Render();
        }
    }
}

void RenderPassMeshMaterial::SetShadowMap(const shared_ptr<RenderTarget> &shadowMap)
{
    m_ShadowMap = shadowMap;
    if (m_ShadowMap != nullptr)
    {
        m_ShadowMap->Bind();
    }
}

void RenderPassMeshMaterial::CollectRenderItems()
{
    m_Items.clear();
    auto root = SceneManager::Instance->GetRoot();
    BuildRenderQueueVisitGameObject(root.get());
}

void RenderPassMeshMaterial::BuildRenderQueueVisitGameObject(GameObject* node)
{
    auto meshRender = node->GetComponent<MeshRenderCmpt>();
    if (meshRender != nullptr)
    {
        m_Items.push_back(meshRender->GetRenderItem());
    }
    for(auto& child : node->GetTransform()->GetChildren())
    {
        BuildRenderQueueVisitGameObject(child->GetOwner());
    }
}

void RenderPassMeshMaterial::CreateCoordAxisRenderItem()
{
    Vector3f pointsLeftHand[12] =
    {
            {0, 0, 0}, {2000, 0, 0},
            {0, 0, 0}, {-2000, 0, 0},
            {0, 0, 0}, {0, 2000, 0},
            {0, 0, 0}, {0, -2000, 0},
            {0, 0, 0}, {0, 0, 2000},
            {0, 0, 0}, {0, 0, -2000}
    };

    Vector4f colors[6] =
    {
            {1, 0, 0, 1},
            {0.5, 0, 0, 1},
            {0, 1, 0, 1},
            {0, 0.5, 0, 1},
            {0, 0, 1, 1},
            {0, 0, 0.5, 1}
    };

    auto vbp = make_shared<VertexBufferCreateParam>();
    vbp->m_Format = VertexBufferFormat::F3;
    vbp->m_ElementCount = 12;
    vbp->m_VertexData = pointsLeftHand;

    unsigned int** indices = new unsigned int* [6];

    auto mp = make_shared<MeshCreateParam>();
    for (int i = 0; i < 6; ++i)
    {
        indices[i] = new unsigned int[2];
        indices[i][0] = i * 2;
        indices[i][1] = i * 2 + 1;
        auto ibp = make_shared<IndexBufferCreateParam>();
        ibp->m_StartIndex = 0;
        ibp->m_Format = IndexBufferFormat::UINT32;
        ibp->m_ElementCount = 2;
        ibp->m_IndexData = indices[i];

        auto smp = make_shared<SubMeshCreateParam>();
        smp->m_PositionCreateParam = vbp;
        smp->m_IndexCreateParam = ibp;
        smp->m_MaterialIndex = i;
        smp->m_PrimitiveType = PrimitiveType::Line;
        mp->m_SubMeshCreateParam.push_back(smp);
    }
    auto mesh = Renderer::Instance->CreateMesh(mp);
    auto item = Renderer::Instance->CreateRenderItem(mesh, nullptr);
    item->AllocateMaterials(6);
    for (int i = 0; i < 6; ++i)
    {
        auto material = Renderer::Instance->CreateMaterial("Color_Axis", MaterialType::MeshRender);
        material->SetVector4("BaseColor", colors[i]);
        item->SetMaterial(i, material);
    }

    m_AxisRenderItems.push_back(item);
    delete[] indices;
}

void RenderPassMeshMaterial::ResizeResolution(int width, int height)
{
    m_MainRT->Resize(width, height);
}

const std::shared_ptr<RenderTarget> &RenderPassMeshMaterial::GetMainRT()
{
    return m_MainRT;
}





