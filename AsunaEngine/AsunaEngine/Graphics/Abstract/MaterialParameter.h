//
// Created by xiao on 2022/4/18.
//

#pragma once

namespace asuna
{
    enum class MaterialParameterType
    {
        Unknown,
        Float,
        Vector4,
        Matrix,
        Texture2D,
    };

    enum class TextureShaderType
    {
        VS,
        PS,
    };

    struct MaterialParameter
    {
        MaterialParameterType m_Type;
        int m_Offset;
        TextureShaderType m_ShaderType;
    };
}

