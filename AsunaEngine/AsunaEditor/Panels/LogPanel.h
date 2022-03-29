//
// Created by xiao on 2022/3/28.
//

#pragma once
#include "BasePanel.h"

namespace asuna
{
    class LogPanel : public BasePanel
    {
    public:
        void Initialize() override;
        void Render() override;
    };

}

