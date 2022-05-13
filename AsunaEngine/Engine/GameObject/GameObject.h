//
// Created by xiao on 2022/3/30.
//
#pragma once
#include <memory>
#include <string>
#include "Component/TransformCmpt/TransformCmpt.h"
#include <map>
#include <typeindex>

namespace asuna
{
    class GameObject
    {
    public:
        GameObject() = delete;
        explicit GameObject(const std::string& name);
        ~GameObject();

    public:

        template<typename T, typename = std::enable_if_t<std::is_base_of_v<Component, T>>>
        std::shared_ptr<T> AddComponent()
        {
            auto cmpt = make_shared<T>(this);
			cmpt->Initialize();

            auto base = std::dynamic_pointer_cast<Component>(cmpt);
            auto pair = std::make_pair(std::type_index(typeid(T)), base);
            m_Components.insert(pair);
            return cmpt;
        }

        template<typename T, typename = std::enable_if_t<std::is_base_of_v<Component, T>>>
        std::shared_ptr<T> GetComponent()
        {
            auto it = m_Components.find(std::type_index(typeid(T)));
            if (it == m_Components.end()) return nullptr;
            return std::dynamic_pointer_cast<T>(it->second);
        }

        template<typename T, typename = std::enable_if_t<std::is_base_of_v<Component, T>>>
        void RemoveComponent()
        {
            auto it = m_Components.find(std::type_index(typeid(T)));
            if (it == m_Components.end())
            {
                return;
            }
            auto cmpt = std::dynamic_pointer_cast<T>(it->second);
			cmpt->Finalize();
            m_Components.erase(std::type_index(typeid(T)));
        }

        void Tick()
        {
            m_TransformCmpt->Tick();
            for(auto cmpt : m_Components)
            {
                cmpt.second->Tick();
            }
        }


    public:
        std::string GetName() { return m_Name;}
        void SetName(const std::string& name) {m_Name = name;}

        TransformCmpt* GetTransform() {return m_TransformCmpt;}


    private:
        std::string m_Name;
        TransformCmpt* m_TransformCmpt;
        std::multimap<std::type_index, std::shared_ptr<Component>> m_Components;
    };
}



