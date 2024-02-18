#include "enxpch.h"

#include "Application.h"
#include "glad/glad.h"

namespace EngineX
{
    Application* Application::s_Instance = nullptr;

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

    Application::Application()
    {
        ENX_ASSERT(!s_Instance, "Application instance already exists!")
        s_Instance = this;

        m_Window = Scope<Window>(Window::Create());
        m_Window->SetEventCallBack(ENX_BIND_EVENT_FN(Application::OnEvent));

        m_ImGuiLayer = new ImGuiLayer();
        InsertOverlay(m_ImGuiLayer);

        // TODO: Most if not all of this rendering stuff needs to be moved out of here at a later date!
        
        // -------------------------------Rendering A Triangle-------------------------------
        
        // Generate a vertex array object (VAO) and bind it
        // VAOs encapsulate vertex attribute configurations and buffer bindings.
        // Binding a VAO means that subsequent vertex attribute settings and buffer bindings will be stored in this VAO.
        glGenVertexArrays(1, &m_VertexArray);
        glBindVertexArray(m_VertexArray);

        // Define the vertex data
        // This array contains the vertex positions for a triangle.
        // Each vertex has three components: x, y, and z coordinates.
        // It also contains rgba values for a given indices
        float vertices[3 * 7] =
        {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.0f, 0.4f, 1.0f, // Lower left with Redish color
            0.5f, -0.5f, 0.0f, 0.4f, 0.8f, 0.0f, 1.0f, // Lower right with greenish color
            0.0f, 0.5f, 0.0f, 0.0f, 0.4f, 0.8f, 1.0f // Upper middle with blueish color
        };

        m_VertexBuffer.reset(VertexBuffer::Create(sizeof(vertices), vertices));

        { // Destroy it after executing it
            BufferLayout layout =
            {
                {ShaderDataType::Float3, "a_Position"},
                {ShaderDataType::Float4, "a_Color"},
            };

            m_VertexBuffer->SetLayout(layout);
        }
        
        uint32_t elementIndex = 0;

        const auto& layout = m_VertexBuffer->GetLayout();
        
        for (const auto& element : layout)
        {
            // Enable the vertex attribute array at index 0
            // This enables the vertex attribute array at index 0 (position attribute).
            glEnableVertexAttribArray(elementIndex);

            // Specify the layout of the vertex attribute array at index 0
            // (position attribute in this case)
            // This describes how the data in the VBO is laid out for the position attribute.
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


        // Define the indices for indexed rendering
        // These indices specify the order in which vertices are used to form primitives (e.g., triangles).
        uint32_t indices[3] = {0, 1, 2};

        m_IndexBuffer.reset(IndexBuffer::Create(std::size(indices), indices));

        // --------------------------------------------------------------------------------------------

        // -----------------------------------Shading the triangle-------------------------------------
        
        std::string vertexSource = R"(
            #version 330 core
            
            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            out vec3 v_Position;
            out vec4 v_Color;

            void main()
            {
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = vec4(a_Position, 1.0);
            }    
        )";

        std::string fragmentSource = R"(
            #version 330 core
            
            layout(location = 0) out vec4 color;

            in vec3 v_Position;
            in vec4 v_Color;

            void main()
            {
                color = v_Color;
            }    
        )";

        m_Shader = CreateScope<Shader>(vertexSource, fragmentSource);

        // --------------------------------------------------------------------------------------------
    }

    Application::~Application() = default;

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(ENX_BIND_EVENT_FN(OnWindowClose));

        // Handle events from the last index -> first index (See LayerStack.h for information!)
        for (auto iterator = m_Layerstack.end(); iterator != m_Layerstack.begin();)
        {
            // We begin at the index just beyond the last, so we have to subtract one to call the last
            (*--iterator)->OnEvent(e);

            // If the event gets handled break out of the loop. So we don't chain the event
            if (e.Handled)
            {
                break;
            }
        }
    }

    void Application::Run()
    {
        while (m_Running)
        {
            // Clears the Color Buffer .. Kind of like refreshing the screen
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            m_Shader->Bind();
            glBindVertexArray(m_VertexArray);
            glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

            // NOTE: Currently we are updating our layers first and then our window .. Might change in the future

            // Update our layers from first index -> last index
            for (Layer* layer : m_Layerstack)
            {
                layer->OnUpdate();
            }

            m_ImGuiLayer->Begin();
            for (Layer* layer : m_Layerstack)
            {
                layer->OnImGuiRender();
            }
            m_ImGuiLayer->End();

            m_Window->OnUpdate();
        }


        ENX_ENGINE_TRACE("Shutting down ...");
    }

    void Application::InsertLayer(Layer* layer)
    {
        m_Layerstack.InsertLayer(layer);
        layer->OnAttach();
    }

    void Application::InsertOverlay(Layer* layer)
    {
        m_Layerstack.InsertOverlay(layer);
        layer->OnAttach();
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;

        return true;
    }
}
