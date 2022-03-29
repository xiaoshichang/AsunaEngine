#pragma once
#include "AsunaEngine/Application/Windows/WindowsApplication.h"

namespace asuna
{
	class WindowsPlayer : public WindowsApplication
	{
	public:
		virtual void Initialize(ApplicationInitParam param) override;
        virtual void Render() override;

	};
}