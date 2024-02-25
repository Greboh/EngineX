#pragma once
#include "EngineX/Rendering/Texture.h"
#include <glad/glad.h>

namespace EngineX
{
    class OpenGLTexture : public Texture2D
    {
    public:
        OpenGLTexture(const std::string& filePath);
        virtual ~OpenGLTexture();

        void Bind(uint32_t slot = 0) const override;

    private:
        std::string m_Path;
        uint32_t m_Width, m_Height;
        RenderID m_RenderID;
    };
}
