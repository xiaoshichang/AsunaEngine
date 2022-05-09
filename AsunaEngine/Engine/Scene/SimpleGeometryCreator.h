//
// Created by xiao on 2022/4/19.
//
#pragma once

#include <memory>
#include "../Graphics/Abstract/Mesh.h"

namespace asuna
{
    class SimpleGeometryCreator
    {
    public:
        static std::shared_ptr<Mesh> CreatePlane();
        static std::shared_ptr<Mesh> CreateCube();
    };
}



