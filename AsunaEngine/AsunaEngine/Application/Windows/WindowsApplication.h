#pragma once
#include <Windows.h>
#include "../Application.h"
#include "../../Graphics/RenderAPIType.h"


namespace asuna
{
	class WindowsApplication : public Application
	{
	public:
		void Initialize(ApplicationInitParam param) override;
		void Finalize() override;
		void Tick() override;

	public:
		HWND GetWindowsHandler() noexcept;

	private:
		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		void InitMainWindow(int width, int height);
		void InitRenderer(RenderAPIType api, int width, int height);

	private:
		HWND m_HWND;
	};
}


