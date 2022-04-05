//
// Created by xiao on 2022/3/31.
//

#include "SceneManager.h"
#include "../Graphics/Renderer.h"
#include "../Foundation/Logger/Logger.h"

using namespace asuna;
using namespace std;

SceneManager* SceneManager::Instance = new SceneManager();

SceneManager::SceneManager()
{
}

void SceneManager::Initialize()
{
    m_Root = std::make_shared<GameObject>("Root");
    m_ConstantBufferPerScene = Renderer::Current->CreateConstantBuffer(ConstantBufferDataType::PerScene);
}

void SceneManager::Finalize()
{
    m_Root = nullptr;
    m_ConstantBufferPerScene = nullptr;
}

void SceneManager::Tick()
{
    TickGameObject(m_Root.get());
    UpdateConstantBufferPerScene();
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
    auto data = (ConstantBufferDataPerScene*)m_ConstantBufferPerScene->GetData();
    auto viewMatrix = mainCamera->GetViewMatrix();
    auto projectionMatrix = mainCamera->GetProjectionMatrix();
    data->m_ViewMatrix = viewMatrix;
    data->m_ProjectionMatrix = projectionMatrix;
    data->m_VP = projectionMatrix * viewMatrix;
}









