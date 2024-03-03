#pragma once
#include "EditorCamera.h"
#include "RenderAPI.h"
#include "Shader.h"

namespace EngineX
{
    class Render
    {
    public:
        static void OnWindowResize(uint32_t width, uint32_t height);

        static void BeginScene(EditorCamera& camera);
        static void EndScene();

        static void Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader);
        
        inline static RenderAPI::API GetAPI() { return RenderAPI::GetAPI(); }

    private:
        struct Data
        {
            glm::mat4 ViewProjectionMatrix;
        };

        static Data* m_SceneData;
    };
}
