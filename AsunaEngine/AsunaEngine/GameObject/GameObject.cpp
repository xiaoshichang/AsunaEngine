//
// Created by xiao on 2022/3/30.
//

#include "GameObject.h"
#include "../Scene/SceneManager.h"
using namespace asuna;

GameObject::GameObject(const std::string &name) :
    m_Name(name),
    m_TransformCmpt(nullptr)
{
    m_TransformCmpt = new TransformCmpt(this);
    m_TransformCmpt->Initialize();
}

GameObject::~GameObject()
{
    m_TransformCmpt->Finalize();
    delete m_TransformCmpt;
    m_TransformCmpt = nullptr;
}
