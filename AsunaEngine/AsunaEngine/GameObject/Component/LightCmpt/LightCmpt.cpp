//
// Created by xiao on 2022/4/21.
//

#include "LightCmpt.h"
#include "../../../Scene/SceneManager.h"

using namespace asuna;

void LightCmpt::Initialize()
{
    SceneManager::Instance->RegisterLight(this);
}

void LightCmpt::Finalize()
{
    SceneManager::Instance->UnregisterLight(this);
}

void LightCmpt::Tick()
{

}
