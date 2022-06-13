//
//  OpenGLShaderUniform.hpp
//  iKan
//
//  Created by iKan on 23/05/22.
//

#pragma once

#include "Renderer/Utils/ShaderUniform.hpp"

namespace iKan {
    
    class OpenGLShader;
    
    /// Stores the type of shader Uniform Resources. Non premitive data types like Sampler
    class OpenGLShaderResourceDeclaration : public ShaderResourceDeclaration {
    public:
        enum class Type { NONE, TEXTURE2D, TEXTURECUBE };
        
        // Member functions
        OpenGLShaderResourceDeclaration(Type type, const std::string& name, uint32_t count);
        virtual ~OpenGLShaderResourceDeclaration() = default;
        
        const std::string& GetName() const override { return m_Name; }
        uint32_t GetRegister() const override { return m_Register; }
        uint32_t GetCount() const override { return m_Count; }
        
        Type GetType() const { return m_Type; }
        
        // Member Variables
        static Type StringToType(const std::string& type);
        static std::string TypeToString(Type type);
        
    private:
        std::string m_Name;
        uint32_t    m_Register;
        uint32_t    m_Count;
        Type        m_Type;
        
        friend class OpenGLShader;
    };
    
    /// Shader Uniform buffer implementation. Stores the Type and count of data (single data from or outside struct)
    class OpenGLShaderUniformDeclaration : public ShaderUniformDeclaration {
    public:
        enum class Type { NONE, FLOAT32, VEC2, VEC3, VEC4, MAT3, MAT4, INT32, BOOLEAN, STRUCT };
        
        // Member functions
        OpenGLShaderUniformDeclaration(ShaderDomain domain, Type type, const std::string& name, uint32_t count = 1);
        OpenGLShaderUniformDeclaration(ShaderDomain domain, ShaderStruct* uniformStruct, const std::string& name, uint32_t count = 1);
        
        virtual ~OpenGLShaderUniformDeclaration() = default;
        
        const std::string& GetName() const override { return m_Name; }
        
        uint32_t GetSize() const override { return m_Size; }
        uint32_t GetCount() const override { return m_Count; }
        uint32_t GetOffset() const override { return m_Offset; }
        
        ShaderDomain GetDomain() const override { return m_Domain; }
        
        uint32_t GetAbsoluteOffset() const { return m_Struct ? m_Struct->GetOffset() + m_Offset : m_Offset; }
        int32_t GetLocation(uint32_t index = 0) const { return m_Location[index]; }
        
        Type GetType() const { return m_Type; }
        bool IsArray() const { return m_Count > 1; }
        
        const ShaderStruct& GetShaderUniformStruct() const { IK_CORE_ASSERT(m_Struct, ""); return *m_Struct; }
        
        // Static Functions
        static uint32_t SizeOfUniformType(Type type);
        static Type StringToType(const std::string& type);
        static std::string TypeToString(Type type);
        
    protected:
        void SetOffset(uint32_t offset) override;
        
    private:
        std::string m_Name;
        
        uint32_t m_Size;
        uint32_t m_Count;
        uint32_t m_Offset;
        
        Type m_Type;
        ShaderDomain m_Domain;
        ShaderStruct* m_Struct;
        
        mutable std::vector<int32_t> m_Location;
        
        friend class OpenGLShader;
        friend class OpenGLShaderUniformBufferDeclaration;
    };
    
    /// Store all the Uniform in a vector
    class OpenGLShaderUniformBufferDeclaration : public ShaderUniformBufferDeclaration {
    public:
        OpenGLShaderUniformBufferDeclaration(const std::string& name, ShaderDomain domain);
        virtual ~OpenGLShaderUniformBufferDeclaration();
        
        void PushUniform(OpenGLShaderUniformDeclaration* uniform);
        
        const std::string& GetName() const override { return m_Name; }
        
        uint32_t GetRegister() const override { return m_Register; }
        uint32_t GetSize() const override { return m_Size; }
        
        const std::vector<ShaderUniformDeclaration*>& GetUniformDeclarations() const override { return m_Uniforms; }
        ShaderUniformDeclaration* FindUniform(const std::string& name) override;
        
        ShaderDomain GetDomain() const { return m_Domain; }
        
    private:
        std::string m_Name;
        
        uint32_t m_Register;
        uint32_t m_Size;
        
        std::vector<ShaderUniformDeclaration*> m_Uniforms;
        ShaderDomain m_Domain;
        
        friend class Shader;
        friend class OpenGLShader;
    };
    
}
