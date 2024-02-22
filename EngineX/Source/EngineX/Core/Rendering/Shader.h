#pragma once

#include <string>

namespace EngineX
{
    class Shader
    {
    public:
        Shader(const std::string& vertexSource, const std::string& fragmentSource);
        ~Shader();

        void Bind() const;
        void Unbind() const;

    private:
        RenderID m_RendererID;
    };

    
}
