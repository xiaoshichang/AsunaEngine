//
// Created by xiao on 2022/3/28.
//

#include "LogPanel.h"
#include <imgui.h>

using namespace asuna;

void LogPanel::Initialize()
{

}

void LogPanel::Render()
{
    ImGui::Begin("Log Window", &m_Showing);

    ImGui::End();
}

void LogPanel::Finalize()
{

}

