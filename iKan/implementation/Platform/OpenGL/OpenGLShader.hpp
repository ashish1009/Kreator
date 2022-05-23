//
//  OpenGLShader.hpp
//  iKan
//
//  Created by iKan on 21/05/22.
//

#pragma once

#include "Renderer/Graphics/Shader.hpp"
#include "Platform/OpenGL/OpenGLShaderUniform.hpp"
#include <glad/glad.h>

namespace iKan {

    /// Implementation for Compiling Open GL Shader
    class OpenGLShader : public Shader {
    public:
        // Constants
        static constexpr uint32_t MaxShaderSupported = 3;

        // Member functions
        OpenGLShader(const std::string& filepath);
        virtual ~OpenGLShader();
        
        void Bind() const override;
        void Unbind() const override;
        
        const std::string& GetName() const override { return m_Name; }
        const std::string& GetFilePath() const override { return m_AssetPath; }
        RendererID GetRendererID() const override { return m_RendererID; }
        
        // Shader Uniforms
        void SetVSMaterialUniformBuffer(const Buffer& buffer) override;
        void SetFSMaterialUniformBuffer(const Buffer& buffer) override;
        void SetGSMaterialUniformBuffer(const Buffer& buffer) override;
        
        bool HasVSMaterialUniformBuffer() const override { return (bool)m_VSMaterialUniformBuffer; }
        bool HasFSMaterialUniformBuffer() const override { return (bool)m_FSMaterialUniformBuffer; }
        bool HasGSMaterialUniformBuffer() const override { return (bool)m_GSMaterialUniformBuffer; }
        
        const ShaderUniformBufferDeclaration& GetVSMaterialUniformBuffer() const override { return *m_VSMaterialUniformBuffer; }
        const ShaderUniformBufferDeclaration& GetFSMaterialUniformBuffer() const override { return *m_FSMaterialUniformBuffer; }
        const ShaderUniformBufferDeclaration& GetGSMaterialUniformBuffer() const override { return *m_GSMaterialUniformBuffer; }
        
        const std::vector<ShaderResourceDeclaration*>& GetResources() const override { return m_Resources; }
        
        // Attributes
        void SetUniformInt1(const std::string& name, int32_t value) override;
        void SetIntArray(const std::string& name, int32_t* values, uint32_t count) override;
        
        void SetUniformMat4Array(const std::string& name, const glm::mat4& values, uint32_t count) override;
        void SetUniformMat4(const std::string& name, const glm::mat4& value) override;
        void SetUniformMat3(const std::string& name, const glm::mat3& value) override;
        
        void SetUniformFloat1(const std::string& name, float value) override;
        void SetUniformFloat2(const std::string& name, const glm::vec2& value) override;
        void SetUniformFloat3(const std::string& name, const glm::vec3& value) override;
        void SetUniformFloat4(const std::string& name, const glm::vec4& value) override;

    private:
        // Member functions
        void PreprocessFile(const std::string& source);
        void Compile();
        
        // Shader Uniform API
        void Parse();
        void ResolveUniforms();
        
        void ParseUniformStruct(const std::string& block, ShaderDomain domain);
        void ParseUniform(const std::string& statement, ShaderDomain domain);

        ShaderStruct* FindStruct(const std::string& name);

        void ResolveAndSetUniforms(const std::shared_ptr<OpenGLShaderUniformBufferDeclaration>& decl, const Buffer& buffer);
        
        void ResolveAndSetUniform(OpenGLShaderUniformDeclaration* uniform, const Buffer& buffer);
        void ResolveAndSetUniformArray(OpenGLShaderUniformDeclaration* uniform, const Buffer& buffer);
        void ResolveAndSetUniformField(const OpenGLShaderUniformDeclaration& field, std::byte* data, int32_t offset);
        
        // Attribites
        void UploadUniformInt1(int32_t location, int32_t value);
        
        void UploadUniformMat4(int32_t location, const glm::mat4& value);
        void UploadUniformMat3(int32_t location, const glm::mat3& value);
        
        void UploadUniformFloat1(int32_t location, float value);
        void UploadUniformFloat2(int32_t location, const glm::vec2& value);
        void UploadUniformFloat3(int32_t location, const glm::vec3& value);
        void UploadUniformFloat4(int32_t location, const glm::vec4& value);
        
        void UploadUniformMat4Array(uint32_t location, const glm::mat4& values, uint32_t count);
        void UploadUniformStruct(OpenGLShaderUniformDeclaration* uniform, std::byte* buffer, uint32_t offset);

        // Return the Uniform Location
        int32_t GetUniformLocation(const std::string& name);

        // Member variables
        RendererID m_RendererID = 0;
        std::string m_AssetPath = "", m_Name = "";
        std::unordered_map<GLenum, std::string> m_SourceShaderString;
        std::unordered_map<std::string, int32_t> m_LocationMap;
        
        // Shader Uniform Data
        std::vector<ShaderStruct*> m_Structs; // Stores the structure in the shader
        std::vector<ShaderResourceDeclaration*> m_Resources; // Stores the resources of shader like sampler
        std::shared_ptr<OpenGLShaderUniformBufferDeclaration> m_VSMaterialUniformBuffer; // Uniform data buffer of vertex shader
        std::shared_ptr<OpenGLShaderUniformBufferDeclaration> m_FSMaterialUniformBuffer; // Uniform data buffer of pixel shader
        std::shared_ptr<OpenGLShaderUniformBufferDeclaration> m_GSMaterialUniformBuffer; // Uniform data buffer of geometry shader
    };
    
}
