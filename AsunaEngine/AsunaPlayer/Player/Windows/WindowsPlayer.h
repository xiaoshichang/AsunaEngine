#pragma once
#include "AsunaEngine/Application/Windows/WindowsApplication.h"

namespace asuna
{
	class WindowsPlayer : public WindowsApplication
	{
	public:
		void Initialize(std::shared_ptr<ApplicationInitParam> param) override;
        void Render() override;
        void Tick() override;

	};
}