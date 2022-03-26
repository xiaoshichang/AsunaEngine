#pragma once

#include <Windows.h>

namespace asuna
{
	enum class RenderSurfaceType
	{
		Unknown,
		WindowsApplication,
		WPFImage,
	};


	class RenderSurface
	{
	public:
		RenderSurface() :
			Type(RenderSurfaceType::Unknown)
		{}

		virtual ~RenderSurface()
		{
		}

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

		virtual ~RenderSurfaceWindowsApplication()
		{
		}

	public:
		HWND HWND;
	};

}