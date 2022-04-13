
#include "RenderTarget.h"
using namespace asuna;

void RenderTarget::Resize(int width, int height)
{
    m_Desc.width = width;
    m_Desc.height = height;
}
