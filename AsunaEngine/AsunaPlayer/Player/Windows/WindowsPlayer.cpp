#include "WindowsPlayer.h"
#include "AsunaEngine/Graphics/Abstract/Renderer.h"
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
    for (int i = 0; i < 20; ++i)
    {
        mesh->SetMaterial(i, "BaseColor");
        auto material = mesh->GetMaterial(i);
        material->SetVector4("BaseColor", {0.05f * i, 0.05f * i, 0.05f * i, 1.0f});
    }
}

void WindowsPlayer::Render()
{
    Renderer::Current->SetViewPort(0, 0, -1, -1);
    SceneManager::Instance->Render(nullptr);
    Renderer::Current->Present();
}

void WindowsPlayer::Tick()
{
    WindowsApplication::Tick();
    SceneManager::Instance->Tick();
}

void WindowsPlayer::OnWindowSizeChange(int width, int height)
{
    WindowsApplication::OnWindowSizeChange(width, height);
}
