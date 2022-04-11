//
// Created by xiao on 2022/4/11.
//

#pragma once

namespace asuna
{
    enum class DEPTH_WRITE_MASK
    {
        DEPTH_WRITE_MASK_ZERO = 0,
        DEPTH_WRITE_MASK_ALL = 1
    };

    enum class COMPARISON_FUNC
    {
        COMPARISON_NEVER = 1,
        COMPARISON_LESS = 2,
        COMPARISON_EQUAL = 3,
        COMPARISON_LESS_EQUAL = 4,
        COMPARISON_GREATER = 5,
        COMPARISON_NOT_EQUAL = 6,
        COMPARISON_GREATER_EQUAL = 7,
        COMPARISON_ALWAYS = 8
    };


    class DepthStencilState
    {
    public:
        DepthStencilState();
        virtual ~DepthStencilState();

        bool GetDepthEnable() const;
        void SetDepthEnable(bool enable);
        bool GetStencilEnable() const;
        void SetStencilEnable(bool enable);
        DEPTH_WRITE_MASK GetWriteMask() const;
        void SetWriteMask(DEPTH_WRITE_MASK mask);
        COMPARISON_FUNC GetDepthComparison() const;
        void SetDepthComparison(COMPARISON_FUNC func);
        unsigned char GetStencilReadMask() const;
        void SetStencilReadMask(unsigned char mask);
        unsigned char GetStencilWriteMask() const;
        void SetStencilWriteMask(unsigned char mask);

        virtual void Bind() = 0;

    protected:
        bool m_Dirty;
        bool m_DepthEnable;
        DEPTH_WRITE_MASK m_DepthWriteMask;
        COMPARISON_FUNC m_DepthComparisonFunc;
        bool m_StencilEnable;
        unsigned char m_StencilReadMask;
        unsigned char m_StencilWriteMask;

    };
}



