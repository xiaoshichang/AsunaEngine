#pragma once
#include <Windows.h>
#include "../../Application.h"

namespace asuna
{
	class WindowsPlayer : public Application
	{
	public:
		// Í¨¹ý Application ¼Ì³Ð
		virtual void Initialize() override;
		virtual void Finialize() override;
		virtual void Tick() override;
		virtual void Render() override;

	public:
		HWND GetWindowsHandler() noexcept;

	private:
		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		void CreateMainWindow();

	private:
		HWND HWND_;
	};
}


