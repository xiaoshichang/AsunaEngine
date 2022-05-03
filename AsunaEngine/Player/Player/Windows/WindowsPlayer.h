#pragma once
#include "Engine/Application/Windows/WindowsApplication.h"

namespace asuna
{
	class WindowsPlayer : public WindowsApplication
	{
	public:
		void Initialize(std::shared_ptr<ApplicationInitParam> param) override;
        void Render() override;
        void Tick() override;

    protected:
        void OnWindowSizeChange(int width, int height) override;

	};
}