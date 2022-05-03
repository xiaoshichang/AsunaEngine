//
// Created by xiao on 2022/4/1.
//

#include "EditorEventManager.h"

#include <utility>

using namespace asuna;

std::unordered_map<EditorEventType, std::vector<EditorHandler>> EditorEventManager::m_Handlers;

void EditorEventManager::Dispatch(EditorEventType t, const void* param)
{
    if (m_Handlers.find(t) == m_Handlers.end())
    {
        return;
    }
    auto handlers = m_Handlers[t];
    for(const auto& handler : handlers)
    {
        handler.Callback(t, param);
    }
}

void EditorEventManager::Register(EditorEventType t, void *listener, EditorEventCallback callback)
{
    if (m_Handlers.find(t) == m_Handlers.end())
    {
        m_Handlers.insert(std::make_pair(t, std::vector<EditorHandler>()));
    }
    auto handler = EditorHandler();
    handler.Listener = listener;
    handler.Callback = std::move(callback);
    m_Handlers[t].push_back(handler);
}

void EditorEventManager::Unregister(EditorEventType t, void *listener, EditorEventCallback callback)
{
    if (m_Handlers.find(t) == m_Handlers.end())
    {
        return;
    }
    auto& handlers = m_Handlers[t];
    for(auto it = handlers.begin(); it != handlers.end();)
    {
        auto handler = *it;
        if (handler.Listener == listener && handler.Callback.target<EditorEventCallback>() == callback.target<EditorEventCallback>())
        {
            it = handlers.erase(it);
        }
        else
        {
            it++;
        }
    }
}
