#include "enxpch.h"
#include "Render.h"

#include "RenderCommand.h"

namespace EngineX
{
    Render::Data* Render::m_SceneData = new Data;
    
    bool m_LoggedOnce = false;

    void Render::BeginScene(Camera& camera)
    {
        m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
        
        if (m_LoggedOnce)
        {
            return;
        }
        
        ENX_ENGINE_WARN("Render::BeginScene()   is currently not supported!");
    }

    void Render::EndScene()
    {
        if (m_LoggedOnce)
        {
            return;
        }
        
        ENX_ENGINE_WARN("Render::EndScene()     is currently not supported!");
        m_LoggedOnce = true;
    }

    void Render::Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader)
    {
        //TODO: Need to implement being able to handle this insde of here. Will require some refactoring and additions!
        // shader->Bind();
        // shader->UploadUniform("u_Texture", 0);
        // shader->UploadUniform("u_ModelViewProjection",  m_SceneData->ViewProjectionMatrix);        
        // vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
}
