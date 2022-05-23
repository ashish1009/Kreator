//
//  ShaderUniform.hpp
//  iKan
//
//  Created by iKan on 23/05/22.
//

#pragma once

namespace iKan {
    
    class OpenGLShader;
    
    /// Type of shader
    enum class ShaderDomain { None = 0, Vertex = 1, Fragment = 2, Geometry = 3 };

    /// Stores shader uniform elemetns
    class ShaderUniformDeclaration {
    private:
        friend class Shader;
        friend class OpenGLShader;
        friend class ShaderStruct;
        
    public:
        virtual ~ShaderUniformDeclaration() = default;
        
        virtual const std::string& GetName() const = 0;
        virtual uint32_t GetSize() const = 0;
        virtual uint32_t GetCount() const = 0;
        virtual uint32_t GetOffset() const = 0;
        virtual ShaderDomain GetDomain() const = 0;
        
    protected:
        virtual void SetOffset(uint32_t offset) = 0;
    };
    
    /// Store all the Uniform in a vector(buffer) for each shader : Vertex, Fragent, Geomatry
    class ShaderUniformBufferDeclaration {
    public:
        virtual ~ShaderUniformBufferDeclaration() = default;
        
        virtual const std::string& GetName() const = 0;
        virtual uint32_t GetRegister() const = 0;
        virtual uint32_t GetSize() const = 0;
        virtual const std::vector<ShaderUniformDeclaration*>& GetUniformDeclarations() const = 0;
        
        virtual ShaderUniformDeclaration* FindUniform(const std::string& name) = 0;
    };
    
    /// Stores shader uniform structor data (name type of element and structre as string)
    class ShaderStruct {
    private:
        friend class Shader;
        
    public:
        ShaderStruct(const std::string& name)
        : m_Name(name), m_Size(0), m_Offset(0) { }
        
        void SetOffset(uint32_t offset) { m_Offset = offset; }
        
        uint32_t GetSize() const { return m_Size; }
        uint32_t GetOffset() const { return m_Offset; }
        
        const std::string& GetName() const { return m_Name; }
        const std::vector<ShaderUniformDeclaration*>& GetFields() const { return m_Fields; }
        
        void AddField(ShaderUniformDeclaration* field) {
            m_Size += field->GetSize();
            uint32_t offset = 0;
            if (m_Fields.size()) {
                ShaderUniformDeclaration* previous = m_Fields.back();
                offset = previous->GetOffset() + previous->GetSize();
            }
            field->SetOffset(offset);
            m_Fields.push_back(field);
        }
        
    private:
        std::string m_Name;
        
        uint32_t m_Size = 0;
        uint32_t m_Offset = 0;
        
        // Stores each field of structure
        std::vector<ShaderUniformDeclaration*> m_Fields;
        
        friend class OpenGLShader;
    };
    
    /// Stores the Uniform Resource (other than fundamental type or struct)
    class ShaderResourceDeclaration {
    public:
        virtual ~ShaderResourceDeclaration() = default;
        
        virtual const std::string& GetName() const = 0;
        virtual uint32_t GetRegister() const = 0;
        virtual uint32_t GetCount() const = 0;
    };
}
