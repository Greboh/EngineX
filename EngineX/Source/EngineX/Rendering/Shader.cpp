#include "enxpch.h"
#include "Shader.h"

#include "Render.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace EngineX
{
    ShaderProgramSource Shader::ParseShader(const std::string& filePath)
    {
        std::ifstream stream(filePath);

        ENX_ENGINE_INFO(filePath);
        ENX_ENGINE_ASSERT(stream.is_open(), "No shader found at provided filePath! {0}", filePath)

        enum class ShaderType
        {
            NONE = -1, VERTEX = 0, FRAGMENT = 1
        };
        
        std::string line;
        auto type = ShaderType::NONE;
        std::stringstream ss[2];

        while (std::getline(stream, line))
        {
            if (line.find("//type") != std::string::npos)
            {
                if (line.find("vertex") != std::string::npos)
                {
                    type = ShaderType::VERTEX;
                }
                else if (line.find("fragment") != std::string::npos || line.find("pixel"))
                {
                    type = ShaderType::FRAGMENT;
                }
            }
            else
            {
                ss[static_cast<int>(type)] << line << '\n';
            }
        }
        
        return {ss[0].str(), ss[1].str()};
    }

    Ref<Shader> Shader::Create(const std::string& name, const std::string& shaderSource)
    {
        switch (Render::GetAPI())
        {
            case RenderAPI::API::NONE: ENX_ENGINE_ASSERT(false, "RenderAPI::None is currently not supported!")

            case RenderAPI::API::OPENGL: return CreateRef<OpenGLShader>(name, shaderSource);
        }

        ENX_ENGINE_ASSERT(false, "Unknown RenderAPI")
        return nullptr;
    }

    

    void ShaderLibrary::Add(const Ref<Shader>& shader)
    {
        const auto& name = shader->GetName();
        ENX_ENGINE_ASSERT(!Exists(name), "Shader already exists!");

        m_Library[name] = shader;
    }

    Ref<Shader> ShaderLibrary::Get(const std::string& nameOfShader)
    {
        ENX_ENGINE_ASSERT(m_Library.contains(nameOfShader), "Shader doesn't exist!");

        return m_Library[nameOfShader];
    }

    void ShaderLibrary::Load(const std::string& name, const std::string& filepath)
    {
        const auto shader = Shader::Create(name, filepath);
        Add(shader);
    }

    bool ShaderLibrary::Exists(const std::string& nameOfShader) const
    {
        return m_Library.contains(nameOfShader);
    }
}
