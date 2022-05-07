#include "WindowsPlayer.h"
#include "Engine/Graphics/Abstract/Renderer.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/AssetLoader/AssetLoader.h"
#include "Engine/GameObject/Component/MeshRenderCmpt/MeshRenderCmpt.h"

using namespace asuna;

void WindowsPlayer::Initialize(std::shared_ptr<ApplicationInitParam> param)
{
	WindowsApplication::Initialize(param);
    SceneManager::Instance->LoadScene("test");
}

void WindowsPlayer::Render()
{
    Renderer::Instance->SetViewPort(0, 0, -1, -1);
    SceneManager::Instance->Render(nullptr);
    Renderer::Instance->Present();
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
