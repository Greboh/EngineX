#include "enxpch.h"
#include "OpenGLVertexArray.h"

#include "glad/glad.h"

namespace EngineX
{
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float: return GL_FLOAT;
            case ShaderDataType::Float2: return GL_FLOAT;
            case ShaderDataType::Float3: return GL_FLOAT;
            case ShaderDataType::Float4: return GL_FLOAT;
            case ShaderDataType::Matrix3: return GL_FLOAT;
            case ShaderDataType::Matrix4: return GL_FLOAT;
            case ShaderDataType::Int: return GL_INT;
            case ShaderDataType::Int2: return GL_INT;
            case ShaderDataType::Int3: return GL_INT;
            case ShaderDataType::Int4: return GL_INT;
            case ShaderDataType::Bool: return GL_BOOL;
        }
    }
    
    OpenGLVertexArray::OpenGLVertexArray()
    {
        // Generate a vertex array object (VAO) and bind it
        // VAOs encapsulate vertex attribute configurations and buffer bindings.
        glCreateVertexArrays(1, &m_RendererID);
    }

    void OpenGLVertexArray::Bind() const
    {
        // Binding a VAO means that subsequent vertex attribute settings and buffer bindings will be stored in this VAO.
        glBindVertexArray(m_RendererID);
    }

    void OpenGLVertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
    {
        ENX_ENGINE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Missing layout in VBO!")
        
        glBindVertexArray(m_RendererID);
        vertexBuffer->Bind();

        uint32_t elementIndex = 0;

        const auto& layout = vertexBuffer->GetLayout();

        for (const auto& element : layout)
        {
            glEnableVertexAttribArray(elementIndex);
            
            glVertexAttribPointer
            (
                elementIndex,
                element.GetComponentCount(),
                ShaderDataTypeToOpenGLBaseType(element.Type),
                element.Normalized ? GL_TRUE : GL_FALSE,
                layout.GetStride(),
                (void*)element.Offset
            );

            elementIndex++;
        }

        m_VertexBuffers.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
    {
        glBindVertexArray(m_RendererID);
        indexBuffer->Bind();
        
        m_IndexBuffer = indexBuffer;
    }
}
