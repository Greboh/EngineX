﻿#pragma once
#include "EngineX/Core/Rendering/Shader.h"

namespace EngineX
{
    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource);
        OpenGLShader(const std::string& shaderSource);

        ~OpenGLShader() override;
        void Bind() const override;
        void Unbind() const override;
        
        void UploadUniform(const std::string& name, const int& value) override;
        void UploadUniform(const std::string& name, const float& value) override;
        void UploadUniform(const std::string& name, const glm::vec2& value) override;
        void UploadUniform(const std::string& name, const glm::vec3& value) override;
        void UploadUniform(const std::string& name, const glm::vec4& value) override;
        void UploadUniform(const std::string& name, const glm::mat3& value) override;
        void UploadUniform(const std::string& name, const glm::mat4& value) override;

    private:
        void Compile(const uint32_t& vertexShader, const uint32_t& fragmentShader) const;
        void Link(const uint32_t& vertexShader, const uint32_t& fragmentShader) const;

        RenderID m_RendererID;
    };
}
