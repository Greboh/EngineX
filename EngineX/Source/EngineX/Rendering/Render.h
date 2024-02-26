#pragma once
#include "Camera.h"
#include "RenderAPI.h"
#include "Shader.h"

namespace EngineX
{
    class Render
    {
    public:
        static void BeginScene(Camera& camera);
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
