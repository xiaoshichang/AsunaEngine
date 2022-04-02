#pragma once
#include <memory>
#include "BasePanel.h"
#include "AsunaEngine/Scene/SceneManager.h"

namespace asuna
{
	class HierarchyPanel : public BasePanel
	{
    public:
         void Initialize() override;
         void Render() override;
         void Finalize() override;

    private:
        void RenderGameObject(GameObject* gameObject);

    private:
        GameObject* m_SelectedObject = nullptr;
	};
}