//
// Created by xiao on 2022/3/31.
//

#include "SceneManager.h"
#include "../Graphics/Abstract/Renderer.h"
#include "../Foundation/Logger/Logger.h"

using namespace asuna;
using namespace std;

SceneManager* SceneManager::Instance = new SceneManager();

SceneManager::SceneManager() :
        m_Cameras()
{
}

void SceneManager::Initialize()
{
    m_ConstantBufferPerScene = Renderer::Current->CreateConstantBuffer(ConstantBufferDataType::PerFrame, sizeof(ConstantBufferDataPerFrame));
    m_RenderItemQueue = Renderer::Current->CreateRenderItemQueue();
    m_DebugRenderItemQueue = Renderer::Current->CreateRenderItemQueue();
    m_Root = std::make_shared<GameObject>("Root");

    if (m_ShowCoordAxis)
    {
        CreateCoordAxisRenderItem();
    }
}

void SceneManager::Finalize()
{
    m_Root = nullptr;
    m_RenderItemQueue = nullptr;
    m_ConstantBufferPerScene = nullptr;
}

void SceneManager::Tick()
{
    TickGameObject(m_Root.get());
    UpdateConstantBufferPerScene();
}

std::shared_ptr<GameObject> SceneManager::GetRoot()
{
    return m_Root;
}

std::shared_ptr<ConstantBuffer> SceneManager::GetConstantBufferPerScene()
{
    return m_ConstantBufferPerScene;
}

void SceneManager::TickGameObject(GameObject* obj)
{
    obj->Tick();
    for (auto child: obj->GetTransform()->GetChildren())
    {
        TickGameObject(child->GetOwner());
    }
}

std::shared_ptr<GameObject> SceneManager::CreateGameObject(const string &name, const std::shared_ptr<GameObject>& parent)
{
    auto obj = std::make_shared<GameObject>(name);
    if (parent == nullptr)
    {
        obj->GetTransform()->SetParent(m_Root->GetTransform());
        m_Root->GetTransform()->AddChild(obj->GetTransform());
    }
    else
    {
        obj->GetTransform()->SetParent(parent->GetTransform());
        parent->GetTransform()->AddChild(obj->GetTransform());
    }
    m_GameObjects.push_back(obj);
    return obj;
}

void SceneManager::UpdateConstantBufferPerScene()
{
    UpdateCameraMatrix();
}

void SceneManager::AddCamera(CameraCmpt *camera)
{
    m_Cameras.push_back(camera);
}

void SceneManager::RemoveCamera(CameraCmpt *camera)
{
    for(auto it = m_Cameras.begin(); it != m_Cameras.end(); it++)
    {
        auto element = *it;
        if (element == camera)
        {
            m_Cameras.erase(it);
            return;
        }
    }
}

CameraCmpt* SceneManager::GetMainCamera()
{
    if (m_Cameras.empty())
    {
        return nullptr;
    }
    return m_Cameras.at(0);
}

void SceneManager::UpdateCameraMatrix()
{
    auto mainCamera = GetMainCamera();
    if (mainCamera == nullptr)
    {
        Logger::Error("no camera in scene!");
        return;
    }
    auto data = (ConstantBufferDataPerFrame*)m_ConstantBufferPerScene->GetData();
    auto viewMatrix = mainCamera->GetViewMatrix();
    auto projectionMatrix = mainCamera->GetProjectionMatrix();
    data->m_ViewMatrix = viewMatrix;
    data->m_ProjectionMatrix = projectionMatrix;
    data->m_VP = projectionMatrix * viewMatrix;
}

void SceneManager::AddRenderItem(const shared_ptr<RenderItem>& item)
{
    m_RenderItemQueue->AddRenderItem(item);
}

void SceneManager::RemoveRenderItem(const shared_ptr<RenderItem>& item)
{
    m_RenderItemQueue->RemoveRenderItem(item);
}

void SceneManager::Render(const std::shared_ptr<RenderTarget>& rt)
{
    Renderer::Current->SetRenderTarget(rt);
    Renderer::Current->ClearRenderTarget(rt, 0.1f, 0.2f, 0.3f, 1.0f);
    m_RenderItemQueue->Render();
    m_DebugRenderItemQueue->Render();
}

void SceneManager::CreateCoordAxisRenderItem()
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

    Vector3f pointsRightHand[12] =
    {
        {0, 0, 0}, {2000, 0, 0},
        {0, 0, 0}, {-2000, 0, 0},
        {0, 0, 0}, {0, 2000, 0},
        {0, 0, 0}, {0, -2000, 0},
        {0, 0, 0}, {0, 0, -2000},
        {0, 0, 0}, {0, 0, 2000}
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

    if (Renderer::Current->CheckLeftHandRenderAPI())
    {
        vbp->m_VertexData = pointsLeftHand;
    }
    else
    {
        vbp->m_VertexData = pointsRightHand;
    }

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
    auto mesh = Renderer::Current->CreateMesh(mp);
    auto item = Renderer::Current->CreateRenderItem(mesh, nullptr);
    item->AllocateMaterials(6);
    AddRenderItem(item);

    for (int i = 0; i < 6; ++i)
    {
        auto material = Renderer::Current->CreateMaterial("Debug");
        material->SetVector4("BaseColor", colors[i]);
        item->SetMaterial(i, material);
    }
}










