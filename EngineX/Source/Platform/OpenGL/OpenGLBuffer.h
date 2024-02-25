#pragma once
#include "EngineX/Rendering/Buffer.h"

namespace EngineX
{
    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(uint32_t size, float* vertices);

        ~OpenGLVertexBuffer() override;
        void Bind() const override;
        void Unbind() const override;

        inline virtual const BufferLayout& GetLayout() const override {return m_Layout; }
        inline virtual void SetLayout(const BufferLayout& layout) override {m_Layout = layout; }

    private:
        RenderID m_RendererID;
        BufferLayout m_Layout;
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
        RenderID m_RendererID;
        uint32_t m_Count;
    };
}
