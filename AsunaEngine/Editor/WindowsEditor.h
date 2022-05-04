#pragma once
#include "Engine/Application/Windows/WindowsApplication.h"
#include "Panels/HierarchyPanel.h"
#include "Panels/LogPanel.h"
#include "Panels/GamePanel.h"
#include "Panels/InspectorPanel.h"


namespace asuna
{
	class WindowsEditor : public WindowsApplication
	{
	public:
        WindowsEditor();
        void Initialize(std::shared_ptr<ApplicationInitParam> param) override;
        void Tick() override;
		void Render() override;
        void Finalize() override;

        static WindowsEditor* Instance;
        HierarchyPanel m_HierarchyPanel;
        LogPanel m_LogPanel;
        GamePanel m_GamePanel;
        InspectorPanel m_InspectorPanel;

    protected:
        void OnWindowSizeChange(int width, int height) override;

	private:
		void RenderEditorBegin();
        void RenderEditorEnd();
	};
}