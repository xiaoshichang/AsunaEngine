#pragma once
#include <Windows.h>
#include "../Application.h"
#include "../../Graphics/RenderAPIType.h"


namespace asuna
{
	class WindowsApplication : public Application
	{
	public:
		// ͨ�� Application �̳�
		virtual void Initialize() override;
		virtual void Finalize() override;
		virtual void Tick() override;
		virtual void Render() override;

	public:
		HWND GetWindowsHandler() noexcept;

	private:
		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		void InitMainWindow();
		void InitRenderer(RenderAPIType api);

	private:
		HWND m_HWND;
	};
}


