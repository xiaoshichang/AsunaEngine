//
// Created by xiao on 2022/4/12.
//

#include "OpenglDepthStencilState.h"
#include "../../3rd/Glad/include/glad/glad.h"

asuna::OpenglDepthStencilState::OpenglDepthStencilState() = default;

asuna::OpenglDepthStencilState::~OpenglDepthStencilState() = default;

void asuna::OpenglDepthStencilState::Bind()
{
    glEnable(GL_DEPTH_TEST);
}
