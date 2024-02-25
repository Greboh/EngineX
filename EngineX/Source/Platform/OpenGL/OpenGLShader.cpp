#include "enxpch.h"
#include "OpenGLShader.h"

#include <sstream>

#include <glm/gtc/type_ptr.hpp>



namespace EngineX
{
    OpenGLShader::OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource)
    {
        // Create an empty vertex shader handle
        uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);

        // Create an empty fragment shader handle
        uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        // Send the vertex shader source code to GL
        // Note that std::string's .c_str is NULL character terminated.
        const GLchar* source = vertexSource.c_str();
        glShaderSource(vertexShader, 1, &source, 0);

        // Send the fragment shader source code to GL
        // Note that std::string's .c_str is NULL character terminated.
        source = fragmentSource.c_str();
        glShaderSource(fragmentShader, 1, &source, 0);

        Compile(vertexShader, fragmentShader);

        // Vertex and fragment shaders are successfully compiled.
        // Now time to link them together into a program.
        // Get a program object.
        m_RendererID = glCreateProgram();

        Link(vertexShader, fragmentShader);

        // Always detach shaders after a successful link.
        glDetachShader(m_RendererID, vertexShader);
        glDetachShader(m_RendererID, fragmentShader);
    }

    OpenGLShader::OpenGLShader(const std::string& shaderSource)
    {
        const ShaderProgramSource programSource = ParseShader(shaderSource);

        // Create an empty vertex shader handle
        uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);

        // Create an empty fragment shader handle
        uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        // Send the vertex shader source code to GL
        // Note that std::string's .c_str is NULL character terminated.
        const GLchar* source = programSource.VertexSource.c_str();
        glShaderSource(vertexShader, 1, &source, 0);

        // Send the fragment shader source code to GL
        // Note that std::string's .c_str is NULL character terminated.
        source = programSource.FragmentSource.c_str();
        glShaderSource(fragmentShader, 1, &source, 0);

        Compile(vertexShader, fragmentShader);

        // Vertex and fragment shaders are successfully compiled.
        // Now time to link them together into a program.
        // Get a program object.
        m_RendererID = glCreateProgram();

        Link(vertexShader, fragmentShader);

        // Always detach shaders after a successful link.
        glDetachShader(m_RendererID, vertexShader);
        glDetachShader(m_RendererID, fragmentShader);
    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_RendererID);
    }
    
    void OpenGLShader::Bind() const
    {
        glUseProgram(m_RendererID);
    }

    void OpenGLShader::Unbind() const
    {
        glUseProgram(0);
    }

    void OpenGLShader::UploadUniform(const std::string& name, const int& value)
    {
        const auto location = GetUniformLocation(name);
        
        glUniform1i(location, value);
    }

    void OpenGLShader::UploadUniform(const std::string& name, const float& value)
    {
        const auto location = GetUniformLocation(name);
        glUniform1f(location, value);
    }

    void OpenGLShader::UploadUniform(const std::string& name, const glm::vec2& value)
    {
        const auto location = GetUniformLocation(name);
        glUniform2f(location, value.x, value.y);
    }

    void OpenGLShader::UploadUniform(const std::string& name, const glm::vec3& value)
    {
        const auto location = GetUniformLocation(name);
        glUniform3f(location, value.x, value.y, value.z);
    }

    void OpenGLShader::UploadUniform(const std::string& name, const glm::vec4& value)
    {
        const auto location = GetUniformLocation(name);
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::UploadUniform(const std::string& name, const glm::mat3& value)
    {
        const auto location = GetUniformLocation(name);
        glUniformMatrix3fv(location, value.length(), FALSE, value_ptr(value));
    }

    void OpenGLShader::UploadUniform(const std::string& name, const glm::mat4& value)
    {
        const auto location = GetUniformLocation(name);
        glUniformMatrix4fv(location, 1, FALSE, value_ptr(value));
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    void OpenGLShader::Compile(const uint32_t& vertexShader, const uint32_t& fragmentShader) const
    {
        // Compile the vertex shader
        glCompileShader(vertexShader);

        GLint isCompiled = 0;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

            // We don't need the shader anymore.
            glDeleteShader(vertexShader);

            // Use the infoLog as you see fit.
            ENX_ENGINE_ERROR("Vertex shader compilation failure! >> {0}", infoLog.data());
            ENX_ERROR("Vertex shader compilation failure! >> {0}", infoLog.data());
            
            // In this simple program, we'll just leave
            return;
        }



        // Compile the fragment shader
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

            // We don't need the shader anymore.
            glDeleteShader(fragmentShader);
            // Either of them. Don't leak shaders.
            glDeleteShader(vertexShader);

            ENX_ENGINE_ERROR("Fragment shader compilation failure! >> {0}", infoLog.data());
            ENX_ERROR("Fragment shader compilation failure! >> {0}", infoLog.data());
        }
    }

    void OpenGLShader::Link(const uint32_t& vertexShader, const uint32_t& fragmentShader) const
    {
        // Attach our shaders to our program
        glAttachShader(m_RendererID, vertexShader);
        glAttachShader(m_RendererID, fragmentShader);

        // Link our program
        glLinkProgram(m_RendererID);

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(m_RendererID, GL_LINK_STATUS, &isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(m_RendererID);
            // Don't leak shaders either.
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            // Use the infoLog as you see fit.
            ENX_ENGINE_ERROR("Linking shader compilation failure! >> {0}", infoLog.data());
            ENX_ERROR("Linking shader compilation failure! >> {0}", infoLog.data());
            
            // In this simple program, we'll just leave
            return;
        }
    }

    GLint OpenGLShader::GetUniformLocation(const std::string& name) const
    {
        if(m_UniformCache.contains(name))
        {
            return m_UniformCache[name];
        }
        
        const GLint location = glGetUniformLocation(m_RendererID, name.c_str());

        if(location == -1)
        {
            ENX_ENGINE_WARN("Uniform {0} doesn't exist!", name);
        }

        m_UniformCache[name] = location;
        return location;
    }
}
