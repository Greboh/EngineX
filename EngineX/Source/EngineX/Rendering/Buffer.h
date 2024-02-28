#pragma once
#include "EngineX/Core/Core.h"

namespace EngineX
{
    enum class ShaderDataType
    {
        None = 0,
        Float,
        Float2,
        Float3,
        Float4,
        Matrix3,
        Matrix4,
        Int,
        Int2,
        Int3,
        Int4,
        Bool
    };

    static uint32_t ShaderDataTypeSize(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::None:          ENX_ENGINE_ASSERT(false, "ShaderDataType None is not supported!") break;
        case ShaderDataType::Float:         return 4;
        case ShaderDataType::Float2:        return 4 * 2;
        case ShaderDataType::Float3:        return 4 * 3;
        case ShaderDataType::Float4:        return 4 * 4;
        case ShaderDataType::Matrix3:       return 4 * 3 * 3;
        case ShaderDataType::Matrix4:       return 4 * 4 * 4;
        case ShaderDataType::Int:           return 4;
        case ShaderDataType::Int2:          return 4 * 2;
        case ShaderDataType::Int3:          return 4 * 3;
        case ShaderDataType::Int4:          return 4 * 4;
        case ShaderDataType::Bool:          return 1;
        }

        ENX_ENGINE_ASSERT(false, "Unknown ShaderDataType!")
        return 0;
    }
    
    struct BufferElement
    {
        std::string Name;
        ShaderDataType Type;
        uint32_t Size;
        uint32_t Offset;
        bool Normalized;

        BufferElement() = default;

        BufferElement(const ShaderDataType& type, const std::string& name, bool normalized = false)
            : Name(name),
              Type(type),
              Size(ShaderDataTypeSize(type)),
              Offset(0),
              Normalized(normalized)
        {
        }

        uint32_t GetComponentCount() const
        {
            switch (Type)
            {
                case ShaderDataType::None:          ENX_ENGINE_ASSERT(false, "ShaderDataType None is not supported!") break;
                case ShaderDataType::Float:         return 1;
                case ShaderDataType::Float2:        return 2;
                case ShaderDataType::Float3:        return 3;
                case ShaderDataType::Float4:        return 4;
                case ShaderDataType::Matrix3:       return 3 * 3;
                case ShaderDataType::Matrix4:       return 4 * 4;
                case ShaderDataType::Int:           return 1;
                case ShaderDataType::Int2:          return 2;
                case ShaderDataType::Int3:          return 3;
                case ShaderDataType::Int4:          return 4;
                case ShaderDataType::Bool:          return 1;
            }

            ENX_ENGINE_ASSERT(false, "Unknown ShaderDataType!")
            return 0;
        }

        std::string ToString() const
        {
            std::stringstream ss;
            ss << "Name: " << Name << ", Size: " << Size << ", Offset: " << Offset << ", Normalized: " << Normalized <<")";
            return ss.str();
        }

    };

    
    class BufferLayout
    {
    public:

        BufferLayout() {};
        
        BufferLayout(const std::initializer_list<BufferElement>& elements)
            : m_Elements(elements) 
        {
            CalculateOffsetsAndStride();
        }

        inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

        inline const uint32_t& GetStride() const { return m_Stride; }

        // Returns an iterator pointing to the first element of the vector.
        std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
        std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }

        // Returns an iterator pointing to the position just beyond the last element of the vector.
        std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
        std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
        
    private:
        inline void CalculateOffsetsAndStride()
        {
            uint32_t offset = 0;
            m_Stride = 0;
            
            for (auto& element : m_Elements)
            {
                element.Offset = offset;
                offset += element.Size;
                m_Stride += element.Size;
            }
        }
        
        std::vector<BufferElement> m_Elements;
        uint32_t m_Stride = 0;
    };
    
    /**
     * \brief Abstract class. Each API needs to implement this. Creates a VertexBuffer
     */
    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() = default;
        
        /**
         * \brief Creates a VertexBuffer using the current RenderAPI
         * \param size size in bytes of the buffer object's new data
         * \param vertices Specifies a pointer to data that will be copied into it for initialization 
         * \return Platform specific VertexBuffer that is: Created, Bound and filled
         */
        static VertexBuffer* Create(uint32_t size, float* vertices);

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual const BufferLayout& GetLayout() const = 0; 
        virtual void SetLayout(const BufferLayout& layout) = 0;

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
