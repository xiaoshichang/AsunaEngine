//
// Created by xiao on 2022/4/1.
//

#pragma once
#include <unordered_map>
#include <vector>
#include <functional>
#include "EditorEventType.h"

namespace asuna
{
    typedef std::function<void(EditorEventType, const void*)> EditorEventCallback;

    struct EditorHandler
    {
        void* Listener;
        EditorEventCallback Callback;
    };


    class EditorEventManager
    {
    public:
        static void Register(EditorEventType t,  void* listener, EditorEventCallback callback);
        static void Unregister(EditorEventType t, void* listener, EditorEventCallback callback);
        static void Dispatch(EditorEventType t, const void* param);

    private:
        static std::unordered_map<EditorEventType, std::vector<EditorHandler>> m_Handlers;
    };
}




