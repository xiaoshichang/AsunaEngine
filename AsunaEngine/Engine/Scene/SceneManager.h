//
// Created by xiao on 2022/3/31.
//

#pragma once

#include <memory>
#include <vector>
#include "../GameObject/GameObject.h"
#include "../Graphics/Abstract/ConstantBuffer.h"
#include "../GameObject/Component/CameraCmpt/CameraCmpt.h"
#include "../GameObject/Component/LightCmpt/LightCmpt.h"
#include "../Graphics/Abstract/RenderItemQueue.h"
#include "../Graphics/Abstract/RenderItem.h"
#include "../Graphics/Abstract/RenderTarget.h"


namespace asuna
{
    class SceneManager
    {
    public:
        SceneManager();
        static SceneManager* Instance;

        void Initialize();
        void Finalize();
        void Tick();
        void Render(const std::shared_ptr<RenderTarget>& rt);

        void LoadScene(const std::string& path);

        std::shared_ptr<GameObject> GetRoot();
        std::shared_ptr<GameObject> CreateGameObject(const std::string& name, const std::shared_ptr<GameObject>& parent);

        void RegisterCamera(CameraCmpt* camera);
        void UnregisterCamera(CameraCmpt* camera);
        CameraCmpt* GetMainCamera();
        void RegisterLight(LightCmpt* light);
        void UnregisterLight(LightCmpt* light);

        std::shared_ptr<ConstantBuffer> GetConstantBufferPerScene();

    private:
        void TickGameObject(GameObject* obj);
        void UpdateConstantBufferPerScene();
        void UpdateCameraMatrix();
        void UpdateLightData();
        void CreateCoordAxisRenderItem();
        void BuildRenderQueue();
        void BuildRenderQueueVisitGameObject(GameObject *node);

    private:
        std::shared_ptr<GameObject> m_Root = nullptr;
        std::vector<std::shared_ptr<GameObject>> m_GameObjects;

        std::vector<CameraCmpt*> m_Cameras;
        std::vector<LightCmpt*> m_Lights;
        std::shared_ptr<ConstantBuffer> m_ConstantBufferPerScene = nullptr;

        std::shared_ptr<RenderItemQueue> m_RenderItemQueue;

        bool m_ShowCoordAxis = true;
        std::vector<std::shared_ptr<RenderItem>> m_AxisRenderItems;
    };

}

