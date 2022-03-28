#pragma once
#include "AsunaEngine/Application/Windows/WindowsApplication.h"


namespace asuna
{
	class WindowsEditor : public WindowsApplication
	{
	public:
		virtual void Render() override;

	private:
		void RenderEditor();
	};
}