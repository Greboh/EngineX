#pragma once
namespace EngineX
{
    /**
     * \brief Abstract class. Each API needs to implement this. Creates a VertexBuffer
     */
    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() {}
        
        /**
         * \brief Creates a VertexBuffer using the current RenderAPI
         * \param size size in bytes of the buffer object's new data
         * \param vertices Specifies a pointer to data that will be copied into it for initialization 
         * \return Platform specific VertexBuffer that is: Created, Bound and filled
         */
        static VertexBuffer* Create(uint32_t size, float* vertices);

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

    };

    

    /**
     * \brief Abstract class. Each API needs to implement this. Creates a IndexBuffer
     */
    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() {}

        static IndexBuffer* Create(uint32_t size, uint32_t* indices);
        
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        /**
         * \brief Gets the number of elements to be rendered.
         * \return The number of elements to be rendered.
         */
        virtual uint32_t GetCount() const = 0;
        
    };
}
