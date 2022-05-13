//
// Created by xiao on 2022/3/31.
//

#include "SceneManager.h"
#include "../AssetLoader/AssetLoader.h"
#include "../Graphics/Abstract/Renderer.h"
#include "../GameObject/Component/MeshRenderCmpt/MeshRenderCmpt.h"
#include "SimpleGeometryCreator.h"
#include "RenderPass/RenderPassMgr.h"

using namespace asuna;
using namespace std;

SceneManager* SceneManager::Instance = nullptr;

SceneManager::SceneManager() :
        m_Cameras()
{
}

void SceneManager::Initialize()
{
    m_ConstantBufferPerScene = Renderer::Instance->CreateConstantBuffer(ConstantBufferDataType::PerFrame, sizeof(ConstantBufferDataPerFrame));
    m_Root = std::make_shared<GameObject>("Root");
}

void SceneManager::Finalize()
{
    m_Root.reset();
    m_ConstantBufferPerScene.reset();
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
    UpdateLightData();
    UpdateShadowData();
}

void SceneManager::RegisterCamera(CameraCmpt *camera)
{
    m_Cameras.push_back(camera);
}

void SceneManager::UnregisterCamera(CameraCmpt *camera)
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

void SceneManager::UpdateLightData()
{
    auto data = (ConstantBufferDataPerFrame*)m_ConstantBufferPerScene->GetData();
    int spotLightCount = 0;
    int pointLightCount = 0;
    for(auto light : m_Lights)
    {
        if(light->GetLightType() == LightType::Direction)
        {
            data->m_DirectionLight.m_Color = light->GetColor();
            data->m_DirectionLight.m_Direction = light->GetOwner()->GetTransform()->GetForward();
            data->m_DirectionLight.m_Intensity = light->GetIntensity();
        }
        else if (light->GetLightType() == LightType::Spot)
        {
            data->m_SpotLight[spotLightCount].m_Position = light->GetOwner()->GetTransform()->GetPosition();
            data->m_SpotLight[spotLightCount].m_Direction = light->GetOwner()->GetTransform()->GetForward();
            data->m_SpotLight[spotLightCount].m_Color = light->GetColor();
            data->m_SpotLight[spotLightCount].m_Intensity = light->GetIntensity();
        }
        else if (light->GetLightType() == LightType::Point)
        {
            data->m_PointLight[pointLightCount].m_Position = light->GetOwner()->GetTransform()->GetPosition();
            data->m_PointLight[pointLightCount].m_Color = light->GetColor();
            data->m_PointLight[pointLightCount].m_Intensity = light->GetIntensity();
        }
        else
        {
            ASUNA_ASSERT(false);
        }
    }

    for(int i = spotLightCount; i < 4; i++)
    {
        data->m_SpotLight[i].m_Intensity.x = 0;
    }
    for(int i = pointLightCount; i < 4; i++)
    {
        data->m_PointLight[i].m_Intensity.x = 0;
    }
}

void SceneManager::UpdateShadowData()
{
    auto data = (ConstantBufferDataPerFrame*)m_ConstantBufferPerScene->GetData();
    for(auto light : m_Lights)
    {
        if(light->GetLightType() == LightType::Direction)
        {
            data->m_LightViewProj = light->GetLightViewProjMatrix();
        }
    }
}


void SceneManager::RegisterLight(LightCmpt *light)
{
    if (m_Lights.size() >= 5)
    {
        Logger::Warning("Light count must <= 5");
        return;
    }
    m_Lights.push_back(light);
}

void SceneManager::UnregisterLight(LightCmpt *light)
{
    for(auto it = m_Lights.begin(); it != m_Lights.end(); it++)
    {
        auto element = *it;
        if (element == light)
        {
            m_Lights.erase(it);
            return;
        }
    }
}

void SceneManager::Render(const std::shared_ptr<RenderTarget>& rt)
{
    RenderPassMgr::Instance->RenderShadowMap();
    RenderPassMgr::Instance->RenderMeshMaterials(rt);
    //RenderPassMgr::Instance->RenderPostProcess(rt);
}


void SceneManager::LoadScene(const string &path)
{
    auto camera = SceneManager::Instance->CreateGameObject("Camera", nullptr);
    camera->GetTransform()->SetPosition(10, 10, -45);
    camera->AddComponent<CameraCmpt>();


    auto girl = SceneManager::Instance->CreateGameObject("girl", nullptr);
    girl->GetTransform()->SetScale(0.1f, 0.1f, 0.1f);

    auto meshParam = AssetLoader::LoadMesh("Assets\\Models\\asuna.fbx");
    auto mesh = Renderer::Instance->CreateMesh(meshParam);
    auto meshCmpt = girl->AddComponent<MeshRenderCmpt>();
    meshCmpt->SetMesh(mesh);
    for(int i = 0; i < meshParam->m_MaterialCount; i++)
    {
        auto material = Renderer::Instance->CreateMaterial("Diffuse_Mesh", MaterialType::MeshRender);
        meshCmpt->SetMaterial(i, material);
    }

    auto tex = Renderer::Instance->CreateTexture("Assets\\Textures\\asuna_diffuse.jpg");
    auto material = meshCmpt->GetMaterial(0);
    material->SetTexture("MainTex", tex);

    // ground
    auto ground = SceneManager::Instance->CreateGameObject("ground", nullptr);
    ground->GetTransform()->SetScale(1000, 1, 1000);
    ground->GetTransform()->SetPosition(0, -0.1, 0);
    auto groundMesh = SimpleGeometryCreator::CreatePlane();
    auto groundMeshCmpt = ground->AddComponent<MeshRenderCmpt>();
    groundMeshCmpt->SetMesh(groundMesh);
    auto groundMaterial = Renderer::Instance->CreateMaterial("Color_Mesh", MaterialType::MeshRender);
    groundMaterial->SetVector4("BaseColor", Vector4f(0.4f, 0.4f, 0.4f, 1.0f));
    groundMeshCmpt->SetMaterial(0, groundMaterial);

    // cube
    auto cube = SceneManager::Instance->CreateGameObject("cube", nullptr);
    cube->GetTransform()->SetScale(2, 2, 2);
    cube->GetTransform()->SetPosition(0, 5, -10);
    auto cubeMesh = SimpleGeometryCreator::CreateCube();
    auto cubeMeshCmpt = cube->AddComponent<MeshRenderCmpt>();
    cubeMeshCmpt->SetMesh(cubeMesh);
    auto cubeMaterial = Renderer::Instance->CreateMaterial("Color_Mesh", MaterialType::MeshRender);
    cubeMaterial->SetVector4("BaseColor", Vector4f(0.4f, 0.6f, 0.4f, 1.0f));
    cubeMeshCmpt->SetMaterial(0, cubeMaterial);

    auto light = SceneManager::Instance->CreateGameObject("Light", nullptr);
    auto lightCmpt = light->AddComponent<LightCmpt>();
    lightCmpt->SetColor(Color(0.8, 0.8, 0.8, 1));
    lightCmpt->SetIntensity(2);
    lightCmpt->GetOwner()->GetTransform()->SetEuler(-0.87, 1.8, 0);
}
