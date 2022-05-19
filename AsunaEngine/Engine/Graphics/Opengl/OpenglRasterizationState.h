#pragma once
#include "../Abstract/RasterizationState.h"

namespace asuna
{

	class OpenglRasterizationState : public RasterizationState
	{
	public:
		OpenglRasterizationState(const RasterizationStateDesc& desc);
		~OpenglRasterizationState() override;
		void Bind();
	};

}