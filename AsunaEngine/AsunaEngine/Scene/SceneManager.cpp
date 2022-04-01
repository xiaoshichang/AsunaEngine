//
// Created by xiao on 2022/3/31.
//

#include "SceneManager.h"

using namespace asuna;
using namespace std;

SceneManager* SceneManager::Instance = new SceneManager();

SceneManager::SceneManager()
{
}

void SceneManager::Initialize()
{
    m_Root = std::make_shared<GameObject>("Root");
}

void SceneManager::Finalize()
{
    m_Root = nullptr;
}

std::shared_ptr<GameObject> SceneManager::CreateGameObject(const string &name, const std::shared_ptr<GameObject>& parent)
{
    auto obj = std::make_shared<GameObject>(name);
    if (parent == nullptr)
    {
        obj->GetTransform()->SetParent(m_Root->GetTransform());
        m_Root->GetTransform()->AddChild(obj->GetTransform());
    }
    else
    {
        obj->GetTransform()->SetParent(parent->GetTransform());
        parent->GetTransform()->AddChild(obj->GetTransform());
    }
    m_GameObjects.push_back(obj);
    return obj;
}

