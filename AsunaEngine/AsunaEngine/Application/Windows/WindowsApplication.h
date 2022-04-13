#pragma once
#include "../../Foundation/Platform/Platform.h"
#ifdef ASUNA_PLATFORM_WINDOWS

#include <Windows.h>
#include "../Application.h"
#include "../../Graphics/Abstract/RenderAPIType.h"

namespace asuna
{
	class WindowsApplication : public Application
	{
	public:
		void Initialize(std::shared_ptr<ApplicationInitParam> param) override;
		void Finalize() override;
		void Tick() override;

		HWND GetWindowsHandler() noexcept;
        void SetWindowTitle(char* title);
        void SetupApplicationTitle(std::shared_ptr<ApplicationInitParam> param);

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
#endif

