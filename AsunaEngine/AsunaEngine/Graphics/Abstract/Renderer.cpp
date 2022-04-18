
#include <algorithm>
#include "Renderer.h"

using namespace asuna;
using namespace std;

Renderer* Renderer::Current = nullptr;


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

