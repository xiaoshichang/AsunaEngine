//
// Created by xiao on 2022/4/11.
//

#include "DepthStencilState.h"

using namespace asuna;

DepthStencilState::DepthStencilState():
        m_DepthEnable(true),
        m_StencilEnable(false),
        m_DepthWriteMask(DEPTH_WRITE_MASK::DEPTH_WRITE_MASK_ALL),
        m_DepthComparisonFunc(COMPARISON_FUNC::COMPARISON_LESS),
        m_StencilReadMask(0xff),
        m_StencilWriteMask(0xff),
        m_Dirty(true)
{
}

DepthStencilState::~DepthStencilState() = default;

bool DepthStencilState::GetDepthEnable() const
{
    return m_DepthEnable;
}

void DepthStencilState::SetDepthEnable(bool enable)
{
    m_DepthEnable = enable;
    m_Dirty = true;
}

bool DepthStencilState::GetStencilEnable() const
{
    return m_StencilEnable;
}

void DepthStencilState::SetStencilEnable(bool enable)
{
    m_StencilEnable = enable;
    m_Dirty = true;
}

DEPTH_WRITE_MASK DepthStencilState::GetWriteMask() const
{
    return m_DepthWriteMask;
}

void DepthStencilState::SetWriteMask(DEPTH_WRITE_MASK mask)
{
    m_DepthWriteMask = mask;
    m_Dirty = true;
}

COMPARISON_FUNC DepthStencilState::GetDepthComparison() const
{
    return m_DepthComparisonFunc;
}

void DepthStencilState::SetDepthComparison(COMPARISON_FUNC func)
{
    m_DepthComparisonFunc = func;
    m_Dirty = true;
}

unsigned char DepthStencilState::GetStencilReadMask() const
{
    return m_StencilReadMask;
}

void DepthStencilState::SetStencilReadMask(unsigned char mask)
{
    m_StencilReadMask = mask;
    m_Dirty = true;
}

unsigned char DepthStencilState::GetStencilWriteMask() const
{
    return m_StencilWriteMask;
}

void DepthStencilState::SetStencilWriteMask(unsigned char mask)
{
    m_StencilWriteMask = mask;
    m_Dirty = true;
}


