#include "WindowsPlayer.h"
#include "AsunaEngine/Graphics/Renderer.h"
#include "AsunaEngine/Scene/SceneManager.h"
#include "AsunaEngine/AssetLoader/AssetLoader.h"
#include "AsunaEngine/GameObject/Component/MeshRenderCmpt/MeshRenderCmpt.h"

using namespace asuna;

void WindowsPlayer::Initialize(std::shared_ptr<ApplicationInitParam> param)
{
	WindowsApplication::Initialize(param);

    auto camera = SceneManager::Instance->CreateGameObject("Camera", nullptr);
    camera->GetTransform()->SetPosition(0, 0, 60);
    camera->AddComponent<CameraCmpt>();

    auto girl = SceneManager::Instance->CreateGameObject("girl", nullptr);
    auto mesh = girl->AddComponent<MeshRenderCmpt>();
    mesh->SetMesh("Assets\\Models\\keqin.fbx");
    mesh->SetMaterial("Assets\\Shaders\\triangle.vs", "Assets\\Shaders\\triangle.ps");
}

void WindowsPlayer::Render()
{
    SceneManager::Instance->Render(nullptr);
    Renderer::Current->Present();
}

void WindowsPlayer::Tick()
{
    WindowsApplication::Tick();
    SceneManager::Instance->Tick();
}
