#pragma once
#include <Windows.h>
#include "../Application.h"
#include "../../Graphics/RenderAPIType.h"


namespace asuna
{
	class WindowsApplication : public Application
	{
	public:
		void Initialize(std::shared_ptr<ApplicationInitParam> param) override;
		void Finalize() override;
		void Tick() override;

		HWND GetWindowsHandler() noexcept;

    protected:
        virtual void OnWindowSizeChange(int width, int height);

	private:
		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		void InitMainWindow(int width, int height);
		void InitRenderer(RenderAPIType api, int width, int height);

	private:
		HWND m_HWND;
	};
}


