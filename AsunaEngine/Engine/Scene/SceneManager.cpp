//
// Created by xiao on 2022/3/31.
//

#include "SceneManager.h"
#include "../AssetLoader/AssetLoader.h"
#include "../Graphics/Abstract/Renderer.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Component/MeshRenderCmpt/MeshRenderCmpt.h"
#include "../GameObject/Component/LightCmpt/LightCmpt.h"
#include "SimpleGeometryCreator.h"

using namespace asuna;
using namespace std;

SceneManager* SceneManager::Instance = new SceneManager();

SceneManager::SceneManager() :
        m_Cameras()
{
}

void SceneManager::Initialize()
{
    m_ConstantBufferPerScene = Renderer::Instance->CreateConstantBuffer(ConstantBufferDataType::PerFrame, sizeof(ConstantBufferDataPerFrame));
    m_RenderItemQueue = Renderer::Instance->CreateRenderItemQueue();
    m_Root = std::make_shared<GameObject>("Root");

    CreateCoordAxisRenderItem();
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
    UpdateLightData();
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

void SceneManager::BuildRenderQueueVisitGameObject(GameObject *node)
{
    auto meshRender = node->GetComponent<MeshRenderCmpt>();
    if (meshRender != nullptr)
    {
        m_RenderItemQueue->AddRenderItem(meshRender->GetRenderItem());
    }
    for(auto& child : node->GetTransform()->GetChildren())
    {
        BuildRenderQueueVisitGameObject(child->GetOwner());
    }
}

void SceneManager::BuildRenderQueue()
{
    m_RenderItemQueue->Clear();
    if (m_ShowCoordAxis)
    {
        for(const auto& item : m_AxisRenderItems)
        {
            m_RenderItemQueue->AddRenderItem(item.get());
        }
    }
    BuildRenderQueueVisitGameObject(m_Root.get());
}

void SceneManager::Render(const std::shared_ptr<RenderTarget>& rt)
{
    BuildRenderQueue();
    Renderer::Instance->SetRenderTarget(rt);
    Renderer::Instance->ClearRenderTarget(rt, 0.1f, 0.2f, 0.3f, 1.0f);
    m_RenderItemQueue->Render();
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

    if (Renderer::Instance->CheckLeftHandRenderAPI())
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
    auto mesh = Renderer::Instance->CreateMesh(mp);
    auto item = Renderer::Instance->CreateRenderItem(mesh, nullptr);
    item->AllocateMaterials(6);
    for (int i = 0; i < 6; ++i)
    {
        auto material = Renderer::Instance->CreateMaterial("Color_Axis");
        material->SetVector4("BaseColor", colors[i]);
        item->SetMaterial(i, material);
    }

    m_AxisRenderItems.push_back(item);
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
        auto material = Renderer::Instance->CreateMaterial("Diffuse_Mesh");
        meshCmpt->SetMaterial(i, material);
    }

    auto tex = Renderer::Instance->CreateTexture("Assets\\Textures\\asuna_diffuse.jpg");
    auto material = meshCmpt->GetMaterial(0);
    material->SetTexture("MainTex", tex);

    auto ground = SceneManager::Instance->CreateGameObject("ground", nullptr);
    ground->GetTransform()->SetScale(1000, 1, 1000);
    ground->GetTransform()->SetPosition(0, -0.1, 0);
    auto groundMesh = SimpleGeometryCreator::CreatePlane();
    auto groundMeshCmpt = ground->AddComponent<MeshRenderCmpt>();
    groundMeshCmpt->SetMesh(groundMesh);
    auto groundMaterial = Renderer::Instance->CreateMaterial("Color_Mesh");
    groundMaterial->SetVector4("BaseColor", Vector4f(0.6f, 0.6f, 0.6f, 1.0f));
    groundMeshCmpt->SetMaterial(0, groundMaterial);

    auto light = SceneManager::Instance->CreateGameObject("Light", nullptr);
    auto lightCmpt = light->AddComponent<LightCmpt>();
    lightCmpt->SetColor(Color(0.8, 0.8, 0.7, 1));
    lightCmpt->SetIntensity(2);
    lightCmpt->GetOwner()->GetTransform()->SetEuler(-PI/2 + 0.1, 0, 0);
}





