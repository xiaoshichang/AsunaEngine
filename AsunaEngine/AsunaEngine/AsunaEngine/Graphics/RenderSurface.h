#pragma once

#include <Windows.h>

namespace asuna
{
	enum class RenderSurfaceType
	{
		WindowsApplication,
		WPFImage,
	};


	class RenderSurface
	{
	public:
		RenderSurfaceType Type;
	};


	class RenderSurfaceWindowsApplication : public RenderSurface
	{
	public:
		RenderSurfaceWindowsApplication(HWND hwnd)
		{
			Type = RenderSurfaceType::WindowsApplication;
			HWND = hwnd;
		}
	public:
		HWND HWND;
	};

}