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

void RenderPassMeshMaterial::Render(const std::shared_ptr<RenderTarget>& outputRT)
{
    SceneManager::Instance->GetConstantBufferPerScene()->Bind();
    Renderer::Instance->SetRenderTarget(outputRT);
    Renderer::Instance->ClearRenderTarget(outputRT, 0.1f, 0.2f, 0.3f, 1.0f);
    if (outputRT != nullptr)
    {
        Renderer::Instance->SetViewPort(0, 0, outputRT->GetWidth(), outputRT->GetHeight());
    }
    else
    {
        Renderer::Instance->SetViewPort(0, 0, -1, -1);
    }

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

void RenderPassMeshMaterial::Render()
{
    Render(m_MainRT);
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

    unsigned int indices[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

    auto mp = make_shared<MeshCreateParam>();
    for (int i = 0; i < 6; ++i)
    {
        auto ibp = make_shared<IndexBufferCreateParam>();
        ibp->m_StartIndex = 0;
        ibp->m_Format = IndexBufferFormat::UINT32;
        ibp->m_ElementCount = 2;
        ibp->m_IndexData = indices + 2 * i;
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
}

void RenderPassMeshMaterial::ResizeResolution(int width, int height)
{
    m_MainRT->Resize(width, height);
}

const std::shared_ptr<RenderTarget> &RenderPassMeshMaterial::GetMainRT()
{
    return m_MainRT;
}



