#pragma once
#include "Application.h"

namespace Asuna
{
	class WindowsPlayer : public Application
	{
	public:
		// Í¨¹ý Application ¼Ì³Ð
		virtual void Initialize() override;
		virtual void Finialize() override;
		virtual void Tick() override;
		virtual void Render() override;
	};
}


