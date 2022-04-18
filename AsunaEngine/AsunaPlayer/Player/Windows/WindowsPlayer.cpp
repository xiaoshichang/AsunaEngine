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
    camera->GetTransform()->SetPosition(10, 10, -45);
    camera->AddComponent<CameraCmpt>();

    auto girl = SceneManager::Instance->CreateGameObject("girl", nullptr);
    auto mesh = girl->AddComponent<MeshRenderCmpt>();
    girl->GetTransform()->SetScale(0.1f, 0.1f, 0.1f);
    mesh->SetMesh("Assets\\Models\\asuna.fbx");

    auto tex = Renderer::Current->CreateTexture( "Assets\\Textures\\asuna_diffuse.jpg");
    auto material = mesh->GetMaterial(0);
    material->SetTexture("MainTex", tex);
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
