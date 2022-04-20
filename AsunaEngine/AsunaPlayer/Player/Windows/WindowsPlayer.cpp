#include "WindowsPlayer.h"
#include "AsunaEngine/Graphics/Abstract/Renderer.h"
#include "AsunaEngine/Scene/SceneManager.h"
#include "AsunaEngine/AssetLoader/AssetLoader.h"
#include "AsunaEngine/GameObject/Component/MeshRenderCmpt/MeshRenderCmpt.h"

using namespace asuna;

void WindowsPlayer::Initialize(std::shared_ptr<ApplicationInitParam> param)
{
	WindowsApplication::Initialize(param);
    SceneManager::Instance->LoadScene("test");
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
