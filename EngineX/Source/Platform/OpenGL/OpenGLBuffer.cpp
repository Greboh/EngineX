#include "enxpch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace EngineX
{
    OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size, float* vertices)
    {
        // Generate a vertex buffer object (VBO) and bind it to GL_ARRAY_BUFFER target
        // VBOs store vertex data, such as positions, colors, or texture coordinates.
        glCreateBuffers(1, &m_RendererID);
        OpenGLVertexBuffer::Bind();

        // Fill the vertex buffer with vertex data
        // The data in 'vertices' is copied into the currently bound GL_ARRAY_BUFFER (m_VertexBuffer).
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        glDeleteBuffers(1, &m_RendererID);
    }

    void OpenGLVertexBuffer::Bind() const
    {
        // Binding a VBO to GL_ARRAY_BUFFER means that it will be used to store vertex attribute data.
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    }

    void OpenGLVertexBuffer::Unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t count, uint32_t* indices)
    {
        m_Count = count;
        
        glCreateBuffers(1, &m_RendererID);
        OpenGLIndexBuffer::Bind();

        // Fill the index buffer with data
        // The data in 'indices' is copied into the currently bound GL_ARRAY_BUFFER (m_RendererId).
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        glDeleteBuffers(1, &m_RendererID);
    }

    void OpenGLIndexBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    }

    void OpenGLIndexBuffer::Unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}
