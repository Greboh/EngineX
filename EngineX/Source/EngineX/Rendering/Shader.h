#pragma once

#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

namespace EngineX
{
    struct ShaderProgramSource {
        std::string VertexSource;
        std::string FragmentSource;
    };
    
    class Shader
    {
    public:

        static ShaderProgramSource ParseShader(const std::string& filePath);

        
        /**
         * \brief This should be used if the shaders have a file each
         * \param vertexSource filePath to the vertex shader file
         * \param fragmentSource filePath to the fragment shader file
         * \return The created shader
         */
        static Shader* Create(const std::string& vertexSource, const std::string& fragmentSource);

        
        /**
         * \brief This should be used if the vertex & fragment shader share file
         * \param shaderSource filePath to the shader file.
         * \return The created shader
         */
        static Shader* Create(const std::string& shaderSource);
        
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void UploadUniform(const std::string& name, const int& value) = 0;
        virtual void UploadUniform(const std::string& name, const float& value) = 0;
        virtual void UploadUniform(const std::string& name, const glm::vec2& value) = 0;
        virtual void UploadUniform(const std::string& name, const glm::vec3& value) = 0;
        virtual void UploadUniform(const std::string& name, const glm::vec4& value) = 0;
        virtual void UploadUniform(const std::string& name, const glm::mat3& value) = 0;
        virtual void UploadUniform(const std::string& name, const glm::mat4& value) = 0;
    };

    
}
