//
// Created by xiao on 2022/3/31.
//

#pragma once
#include <memory>
#include <vector>
#include "../GameObject/GameObject.h"

namespace asuna
{
    class SceneManager
    {
    public:
        SceneManager();
        static SceneManager* Instance;

        void Initialize();
        void Finalize();


        std::shared_ptr<GameObject> GetRoot() { return m_Root;}
        std::shared_ptr<GameObject> CreateGameObject(const std::string& name, const std::shared_ptr<GameObject>& parent);


    private:
        std::shared_ptr<GameObject> m_Root;
        std::vector<std::shared_ptr<GameObject>> m_GameObjects;

    };

}

