//
// Created by xiao on 2022/3/30.
//

#pragma once


namespace asuna
{
    class IComponent
    {
    public:
        virtual void Initialize() = 0;
        virtual void Finalize() = 0;
    };



    class Component : public IComponent
    {

    };

}

