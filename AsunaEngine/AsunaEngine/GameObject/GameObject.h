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
        T* AddComponent()
        {
            auto cmpt = (Component*)new T(this);
            auto pair = std::make_pair(std::type_index(typeid(T)), cmpt);
            cmpt->Initialize();
            return dynamic_cast<T*>(m_Components.insert(pair)->second);
        }

        template<typename T, typename = std::enable_if_t<std::is_base_of_v<Component, T>>>
        T* GetComponent()
        {
            auto it = m_Components.find(std::type_index(typeid(T)));
            if (it == m_Components.end()) return nullptr;
            return dynamic_cast<T*>(it->second);
        }

        template<typename T, typename = std::enable_if_t<std::is_base_of_v<Component, T>>>
        void RemoveComponent()
        {
            auto it = m_Components.find(std::type_index(typeid(T)));
            if (it == m_Components.end())
            {
                return;
            }
            auto cmpt = (Component*)it->second;

            m_Components.erase(std::type_index(typeid(T)));
            cmpt->Finalize();
            delete cmpt;

        }


    public:
        std::string GetName() { return m_Name;}
        void SetName(const std::string& name) {m_Name = name;}

        TransformCmpt* GetTransform() {return m_TransformCmpt;}


    private:
        std::string m_Name;
        TransformCmpt* m_TransformCmpt;
        std::multimap<std::type_index, Component*> m_Components;
    };
}



