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
        void Initialize(std::shared_ptr<ApplicationInitParam> param) override;
        void Tick() override;
		void Render() override;
        void Finalize() override;

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