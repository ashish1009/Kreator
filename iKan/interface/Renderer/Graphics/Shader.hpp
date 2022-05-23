//
//  Shader.hpp
//  iKan
//
//  Created by iKan on 21/05/22.
//

#pragma once

#include "Renderer/Utils/Renderer.hpp"

namespace iKan {
    
    // Forward Declaration
    class ShaderUniformBufferDeclaration;
    class ShaderResourceDeclaration;
    
    /// Maximum Texture slot supported by Senderer Shader
    /// Current Open GL Version 4.1 supports only 16 Texture slot in Shader
    /// TODO: Change this value based on the Open GL Version
    static constexpr uint32_t MaxTextureSlotsInShader = 16;

    /// Interface class for Storing Renderer Shader Compiler. Implementation is depending on the
    /// Supported Renerer API.
    class Shader {
    public:
        virtual ~Shader() = default;
        
        /// Bind the Current Shader to GPU
        virtual void Bind() const = 0;
        /// Unbind the Current Shader to GPU
        virtual void Unbind() const = 0;
        
        /// Return the Name of Shader
        virtual const std::string& GetName() const = 0;
        /// Return the File Path of Shader
        virtual const std::string& GetFilePath() const = 0;
        /// return the Renderer ID of Shader
        virtual RendererID GetRendererID() const = 0;
        
        /// Set the Vertex Shader Uniform Buffer
        /// @param buffer Buffer to be set as Vertex Buffer Uniform
        virtual void SetVSMaterialUniformBuffer(const Buffer& buffer) = 0;
        /// Set the Fragment Shader Uniform Buffer
        /// @param buffer Buffer to be set as Fragment Buffer Uniform
        virtual void SetFSMaterialUniformBuffer(const Buffer& buffer) = 0;
        /// Set the Geomatry Shader Uniform Buffer
        /// @param buffer Buffer to be set as Geomatry Buffer Uniform
        virtual void SetGSMaterialUniformBuffer(const Buffer& buffer) = 0;
        
        /// Check if shader have Vertex Shader Uniform Buffer Data
        virtual bool HasVSMaterialUniformBuffer() const = 0;
        /// Check if shader have Fragment Shader Uniform Buffer Data
        virtual bool HasFSMaterialUniformBuffer() const = 0;
        /// Check if shader have Geomatry Shader Uniform Buffer Data
        virtual bool HasGSMaterialUniformBuffer() const = 0;
        
        /// Return the Vertex Shader Uniform Buffer data
        virtual const ShaderUniformBufferDeclaration& GetVSMaterialUniformBuffer() const = 0;
        /// Return the Fragment Shader Uniform Buffer data
        virtual const ShaderUniformBufferDeclaration& GetFSMaterialUniformBuffer() const = 0;
        /// Return the Geomatry Shader Uniform Buffer data
        virtual const ShaderUniformBufferDeclaration& GetGSMaterialUniformBuffer() const = 0;
        
        /// Return all the resources stored in shader
        virtual const std::vector<ShaderResourceDeclaration*>& GetResources() const = 0;
        
        // ----------------------------- Attributs -------------------------------------
        /// Upload the Int value to shader
        /// @param name Name of Uniform
        /// @param value Value of Uniform
        virtual void SetUniformInt1(const std::string& name, int32_t value) = 0;
        /// Upload the Int Array value to shader
        /// @param name Name of Uniform
        /// @param value Value of Uniform
        /// @param count Size of array
        virtual void SetIntArray(const std::string& name, int32_t* values, uint32_t count) = 0;
        
        /// Upload the Matrix 4x4 array value to shader
        /// @param name Name of Uniform
        /// @param value Value of Uniform
        /// @param count Size of Mat4 Array
        virtual void SetUniformMat4Array(const std::string& name, const glm::mat4& values, uint32_t count) = 0;
        
        /// Upload the Matrix 4x4 value to shader
        /// @param name Name of Uniform
        /// @param value Value of Uniform
        virtual void SetUniformMat4(const std::string& name, const glm::mat4& value) = 0;
        /// Upload the Matrix 3x3 value to shader
        /// @param name Name of Uniform
        /// @param value Value of Uniform
        virtual void SetUniformMat3(const std::string& name, const glm::mat3& value) = 0;
        
        /// Upload the Flaot value to shader
        /// @param name Name of Uniform
        /// @param value Value of Uniform
        virtual void SetUniformFloat1(const std::string& name, float value) = 0;
        /// Upload the Vec2 value to shader
        /// @param name Name of Uniform
        /// @param value Value of Uniform
        virtual void SetUniformFloat2(const std::string& name, const glm::vec2& value) = 0;
        /// Upload the Vec3 value to shader
        /// @param name Name of Uniform
        /// @param value Value of Uniform
        virtual void SetUniformFloat3(const std::string& name, const glm::vec3& value) = 0;
        /// Upload the Vec4 value to shader
        /// @param name Name of Uniform
        /// @param value Value of Uniform
        virtual void SetUniformFloat4(const std::string& name, const glm::vec4& value) = 0;
        
        /// Create Shader Instance based on the Suported API
        /// @param path Absolute Path of shader
        static std::shared_ptr<Shader> Create(const std::string& path);
    };
    
}
