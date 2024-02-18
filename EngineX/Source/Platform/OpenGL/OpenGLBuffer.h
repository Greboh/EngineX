#pragma once
#include "EngineX/Rendering/Buffer.h"
#include "glad/glad.h"

namespace EngineX
{
    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(uint32_t size, float* vertices);

        ~OpenGLVertexBuffer() override;
        void Bind() const override;
        void Unbind() const override;

    private:
        unsigned int m_RendererID;
    };

    class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(uint32_t count, uint32_t* indices);

        ~OpenGLIndexBuffer() override;
        void Bind() const override;
        void Unbind() const override;
        
        inline uint32_t GetCount() const override {return m_Count;}

    private:
        unsigned int m_RendererID;
        uint32_t m_Count;
    };
}
