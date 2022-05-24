//
//  Material.hpp
//  iKan
//
//  Created by iKan on 24/05/22.
//

#pragma once

#include "Renderer/Graphics/Shader.hpp"
#include "Renderer/Graphics/Texture.hpp"
#include "Renderer/Utils/ShaderUniform.hpp"

namespace iKan {
    
    class MaterialInstance;
    
    /// Stores the material
    class Material {
    public:
        /// constructor
        /// @param shader shader instance
        Material(const std::shared_ptr<Shader>& shader);
        /// Destructor
        virtual ~Material();
        
        /// Upload the date to material of type T
        /// @param name name of uniform store in shader
        /// @param value value
        template <typename T> void Set(const std::string& name, const T& value) {
            auto decl = FindUniformDeclaration(name);
            auto& buffer = GetUniformBufferTarget(decl);
            buffer.Write((std::byte*)& value, decl->GetSize(), decl->GetOffset());
            
            OnMaterialValueUpdated(decl);
        }
        
        /// Upload the texture to material
        /// @param name name of uniform
        /// @param texture texture data
        void Set(const std::string& name, const std::shared_ptr<Texture>& texture);
        
        /// Bind the material and upload all the shader data.
        /// Bind all the loaded texture to shader slot
        /// Call before rendering any scene
        void Bind();
        /// Unbind the material
        /// Call after rendering any scene
        void Unbind();
        
        /// return the shader binded to Material
        const std::shared_ptr<Shader> GetShader() const;
        
        /// Create instance of Material with Shader instance
        /// @param shader shader instance
        static std::shared_ptr<Material> Create(const std::shared_ptr<Shader>& shader);
        /// Create instance of Material with sahder file path
        /// @param shaderFilePath shader file path
        static std::shared_ptr<Material> Create(const std::string& shaderFilePath);
        
    private:
        // Member Functions
        ShaderUniformDeclaration* FindUniformDeclaration(const std::string& name);
        ShaderResourceDeclaration* FindResourceDeclaration(const std::string& name);
        Buffer& GetUniformBufferTarget(ShaderUniformDeclaration* uniformDeclaration);
        
        void OnMaterialValueUpdated(ShaderUniformDeclaration* decl);
        void AllocateStorage();
        void BindTextures();
        void UnbindTextures();

        // Member Variabls
        std::shared_ptr<Shader> m_Shader;
        std::unordered_set<MaterialInstance*> m_MaterialInstances;
        
        Buffer m_VSUniformStorageBuffer;
        Buffer m_FSUniformStorageBuffer;
        Buffer m_GSUniformStorageBuffer;
        
        std::vector<std::shared_ptr<Texture>> m_Textures;
        
        friend class MaterialInstance;
    };
    
    /// Material of submeshes
    class MaterialInstance {
    public:
        /// Material instance cpnstructor
        /// @param material material reference
        /// @param name name of material
        MaterialInstance(const std::shared_ptr<Material>& material, const std::string& name = "");
        /// Destrcutor
        virtual ~MaterialInstance();
        
        /// Upload the date to material of type T
        /// @param name name of uniform store in shader
        /// @param value value
        template <typename T> void Set(const std::string& name, const T& value) {
            auto decl = m_Material->FindUniformDeclaration(name);
            auto& buffer = GetUniformBufferTarget(decl);
            buffer.Write((std::byte*)& value, decl->GetSize(), decl->GetOffset());
            
            m_OverriddenValues.insert(name);
        }
        
        /// Upload the texture to material
        /// @param name name of uniform
        /// @param texture texture data
        void Set(const std::string& name, const std::shared_ptr<Texture>& texture);
        
        /// Bind the material and upload all the shader data.
        /// Bind all the loaded texture to shader slot
        /// Call before rendering any scene
        void Bind();
        /// Unbind the material
        /// Call after rendering any scene
        void Unbind();
        
        /// return the shader binded to Material
        const std::shared_ptr<Shader> GetShader() const;
        
        /// return the name of material
        const std::string& GetName() const;
        /// Update the name of material
        /// @param string new name of material
        void SetName(const std::string&);

        /// Create instance of material instance with material reference and name
        /// @param material material reference
        /// @param name name of material
        static std::shared_ptr<MaterialInstance> Create(const std::shared_ptr<Material>& material, const std::string& name = "");

    private:
        // Member functions
        void AllocateStorage();
        Buffer& GetUniformBufferTarget(ShaderUniformDeclaration* uniformDeclaration);
        void OnMaterialValueUpdated(ShaderUniformDeclaration* decl);

        // member variables
        std::shared_ptr<Material> m_Material;
        std::string m_Name;
        
        Buffer m_VSUniformStorageBuffer;
        Buffer m_FSUniformStorageBuffer;
        Buffer m_GSUniformStorageBuffer;
        
        std::vector<std::shared_ptr<Texture>> m_Textures;
        
        std::unordered_set<std::string> m_OverriddenValues;

        friend class Material;
    };
    
}
    
