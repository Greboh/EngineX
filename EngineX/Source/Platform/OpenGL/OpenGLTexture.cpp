#include "enxpch.h"
#include "OpenGLTexture.h"

#include <glad/glad.h>

#include "EngineX/Core/Core.h"
#include "stb_image.h"

namespace EngineX
{
    OpenGLTexture::OpenGLTexture(const std::string& filePath)
    {
        int width, height, nrChannels;
        stbi_uc* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
        ENX_ENGINE_ASSERT(data, "Failed to load image!");

        m_Width = width;
        m_Height = height;

        if (data)
        {
            GLenum internalFormat = 0, dataFormat = 0;
            if (nrChannels == 4)
            {
                internalFormat = GL_RGBA8;
                dataFormat = GL_RGBA;
            }
            else if (nrChannels == 3)
            {
                internalFormat = GL_RGB8;
                dataFormat = GL_RGB;
            }

            glCreateTextures(GL_TEXTURE_2D, 1, &m_RenderID);
            glTextureStorage2D(m_RenderID, 1, internalFormat, m_Width, m_Height);
            
            glTextureParameteri(m_RenderID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(m_RenderID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTextureParameteri(m_RenderID, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTextureParameteri(m_RenderID, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glTextureSubImage2D
            (
                m_RenderID,
                0,
                0,
                0,
                m_Width,
                m_Height,
                dataFormat,
                GL_UNSIGNED_BYTE,
                data
            );
            
            stbi_image_free(data);
        }
    }

    OpenGLTexture::~OpenGLTexture()
    {
        glDeleteTextures(1, &m_RenderID);
    }

    void OpenGLTexture::Bind(uint32_t slot) const
    {
        glBindTextureUnit(slot, m_RenderID);
    }
}
