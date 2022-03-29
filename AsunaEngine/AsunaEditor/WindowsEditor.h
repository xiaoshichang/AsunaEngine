#pragma once
#include "AsunaEngine/Application/Windows/WindowsApplication.h"
#include "Panels/HierarchyPanel.h"
#include "Panels/LogPanel.h"
#include "Panels/GamePanel.h"
#include "Panels/InspectorPanel.h"


namespace asuna
{
	class WindowsEditor : public WindowsApplication
	{
	public:
        virtual void Initialize(ApplicationInitParam param) override;
		virtual void Render() override;

	private:
		void RenderEditorBegin();
        void RenderEditorEnd();


    private:
        HierarchyPanel m_HierarchyPanel;
        LogPanel m_LogPanel;
        GamePanel m_GamePanel;
        InspectorPanel m_InspectorPanel;

	};
}