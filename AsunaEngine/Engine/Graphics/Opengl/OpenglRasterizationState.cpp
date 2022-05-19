#include "OpenglRasterizationState.h"
#include "../../3rd/Glad/include/glad/glad.h"
#include "../../Foundation/Platform/Assert.h"

using namespace asuna;
using namespace std;

OpenglRasterizationState::OpenglRasterizationState(const RasterizationStateDesc& desc) :
	RasterizationState(desc)
{

}

OpenglRasterizationState::~OpenglRasterizationState()
{

}

void OpenglRasterizationState::Bind()
{
	if (m_Desc.CullMode == RasterizationCullMode::None)
	{
		glDisable(GL_CULL_FACE);
	}
	else if (m_Desc.CullMode == RasterizationCullMode::Front)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
	}
	else if (m_Desc.CullMode == RasterizationCullMode::Back)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
	else
	{
		ASUNA_ASSERT(false);
	}

	if (m_Desc.FillMode == RasterizationFillMode::Solid)
	{

	}
	else if (m_Desc.FillMode == RasterizationFillMode::WireFrame)
	{

	}
	else
	{
		ASUNA_ASSERT(false);
	}


	if (m_Desc.FrontCounterClockWise)
	{
		glFrontFace(GL_CCW);
	}
	else
	{
		glFrontFace(GL_CW);
	}
}

