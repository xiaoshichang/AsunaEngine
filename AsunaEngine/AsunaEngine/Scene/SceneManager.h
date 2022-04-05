//
// Created by xiao on 2022/3/31.
//

#pragma once

#include <memory>
#include <vector>
#include "../GameObject/GameObject.h"
#include "../Graphics/ConstantBuffer.h"
#include "../GameObject/Component/CameraCmpt/CameraCmpt.h"
#include "../Graphics/RenderItemQueue.h"
#include "../Graphics/RenderItem.h"
#include "../Graphics/RenderTarget.h"


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

        std::shared_ptr<GameObject> GetRoot();
        std::shared_ptr<GameObject> CreateGameObject(const std::string& name, const std::shared_ptr<GameObject>& parent);

        void AddCamera(CameraCmpt* camera);
        void RemoveCamera(CameraCmpt* camera);
        CameraCmpt* GetMainCamera();

        const std::shared_ptr<ConstantBuffer>& GetConstantBufferPerScene();
        void AddRenderItem(const std::shared_ptr<RenderItem>& item);
        void RemoveRenderItem(const std::shared_ptr<RenderItem>& item);

    private:
        void TickGameObject(GameObject* obj);
        void UpdateConstantBufferPerScene();
        void UpdateCameraMatrix();

    private:
        std::shared_ptr<GameObject> m_Root = nullptr;
        std::vector<std::shared_ptr<GameObject>> m_GameObjects;

        std::vector<CameraCmpt*> m_Cameras;
        std::shared_ptr<ConstantBuffer> m_ConstantBufferPerScene = nullptr;

        std::shared_ptr<RenderItemQueue> m_RenderItemQueue;

    };

}

