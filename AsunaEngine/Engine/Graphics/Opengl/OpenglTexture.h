//
// Created by xiao on 2022/4/18.
//

#pragma once

#include <memory>
#include "../../3rd/Glad/include/glad/glad.h"
#include "../Abstract/Texture.h"

namespace asuna
{

    class OpenglTexture : public ImageTexture
    {
    public:
        explicit OpenglTexture(const std::shared_ptr <RawTexture> &rawImage, GLuint texture);
        ~OpenglTexture() override;
        GLuint GetTexture() const {return m_Texture;}

        static std::shared_ptr<Texture> Create(const std::shared_ptr<RawTexture>& rawTexture);

    private:
        GLuint m_Texture;
    };
}



