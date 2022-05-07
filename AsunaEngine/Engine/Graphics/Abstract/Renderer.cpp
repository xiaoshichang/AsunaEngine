
#include <algorithm>
#include "Renderer.h"

using namespace asuna;
using namespace std;

Renderer* Renderer::Instance = nullptr;

Renderer::Renderer() : m_Context(nullptr)
{
    m_APIType = RenderAPIType::None;
}

Renderer::~Renderer() = default;

shared_ptr<RenderContext> asuna::Renderer::GetContext()
{
	return m_Context;
}


bool Renderer::CheckLeftHandRenderAPI() const
{
    switch (m_APIType)
    {
        case RenderAPIType::Directx11:
            return true;
        case RenderAPIType::Opengl:
            return false;
        default:
            ASUNA_ASSERT(false);
            return false;
    }
}

