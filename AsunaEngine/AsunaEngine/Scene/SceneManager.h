//
// Created by xiao on 2022/3/31.
//

#pragma once

#include <memory>
#include <vector>
#include "../GameObject/GameObject.h"
#include "../Graphics/ConstantBuffer.h"
#include "../GameObject/Component/CameraCmpt/CameraCmpt.h"

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

        std::shared_ptr<GameObject> GetRoot() { return m_Root;}
        std::shared_ptr<GameObject> CreateGameObject(const std::string& name, const std::shared_ptr<GameObject>& parent);
        const std::shared_ptr<ConstantBuffer>& GetConstantBufferPerScene(){return m_ConstantBufferPerScene;}
        void AddCamera(CameraCmpt* camera);
        void RemoveCamera(CameraCmpt* camera);
        CameraCmpt* GetMainCamera();


    private:
        void TickGameObject(GameObject* obj);
        void UpdateConstantBufferPerScene();
        void UpdateCameraMatrix();


    private:

        std::shared_ptr<GameObject> m_Root = nullptr;
        std::vector<std::shared_ptr<GameObject>> m_GameObjects;

        std::vector<CameraCmpt*> m_Cameras;
        std::shared_ptr<ConstantBuffer> m_ConstantBufferPerScene = nullptr;



    };

}

