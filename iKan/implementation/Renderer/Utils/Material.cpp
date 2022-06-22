//
//  Material.cpp
//  iKan
//
//  Created by iKan on 24/05/22.
//

#include "Material.hpp"
#include "Renderer/Utils/Renderer.hpp"

using namespace iKan;

/// Create insyacnce of Material
/// @param shader Shader to be binded with this material
std::shared_ptr<Material> Material::Create(const std::shared_ptr<Shader>& shader) {
    return std::make_shared<Material>(shader);
}

/// Create insyacnce of Material
/// @param shader Shader to be binded with this material
std::shared_ptr<Material> Material::Create(const std::string& shaderFilePath) {
    auto shader = Renderer::GetShader(shaderFilePath);
    return std::make_shared<Material>(shader);
}

/// Material Constructor
/// @param shader Shader to be binded with this material
Material::Material(const std::shared_ptr<Shader>& shader) : m_Shader(shader) {
    Renderer::Submit([this]() {
        IK_LOG_SEPARATOR();
        IK_CORE_INFO("Creating Material ...");
        IK_CORE_INFO("    Shader           : {0}", m_Shader->GetName());
        AllocateStorage();
    });
}

/// Material Destructor
Material::~Material() {
    IK_LOG_SEPARATOR();
    IK_CORE_WARN("Desroying Material !!!");
    IK_CORE_WARN("    Shader : {0}", m_Shader->GetName());
}

/// Allocate memory to store shader uniforms
void Material::AllocateStorage() {
    if (m_Shader->HasVSMaterialUniformBuffer()) {
        const auto& vsBuffer = m_Shader->GetVSMaterialUniformBuffer();
        m_VSUniformStorageBuffer.Allocate(vsBuffer.GetSize());
        m_VSUniformStorageBuffer.ZeroInitialize();
        IK_CORE_INFO("    Vertex Buffer    : {0} Bytes", vsBuffer.GetSize());
    }
    
    if (m_Shader->HasFSMaterialUniformBuffer()) {
        const auto& psBuffer = m_Shader->GetFSMaterialUniformBuffer();
        m_FSUniformStorageBuffer.Allocate(psBuffer.GetSize());
        m_FSUniformStorageBuffer.ZeroInitialize();
        IK_CORE_INFO("    Fragment Buffer  : {0} Bytes", psBuffer.GetSize());
    }
    
    if (m_Shader->HasGSMaterialUniformBuffer()) {
        const auto& gsBuffer = m_Shader->GetGSMaterialUniformBuffer();
        m_GSUniformStorageBuffer.Allocate(gsBuffer.GetSize());
        m_GSUniformStorageBuffer.ZeroInitialize();
        IK_CORE_INFO("    Geomatery Buffer : {0} Bytes", gsBuffer.GetSize());
    }
}

/// Find the uniform from shader
/// @param name Uniform name
ShaderUniformDeclaration* Material::FindUniformDeclaration(const std::string& name) {
    if (m_VSUniformStorageBuffer) {
        auto& declarations = m_Shader->GetVSMaterialUniformBuffer().GetUniformDeclarations();
        for (ShaderUniformDeclaration* uniform : declarations) {
            if (uniform->GetName() == name)
                return uniform;
        }
    }
    
    if (m_FSUniformStorageBuffer) {
        auto& declarations = m_Shader->GetFSMaterialUniformBuffer().GetUniformDeclarations();
        for (ShaderUniformDeclaration* uniform : declarations) {
            if (uniform->GetName() == name)
                return uniform;
        }
    }
    
    if (m_GSUniformStorageBuffer) {
        auto& declarations = m_Shader->GetGSMaterialUniformBuffer().GetUniformDeclarations();
        for (ShaderUniformDeclaration* uniform : declarations) {
            if (uniform->GetName() == name)
                return uniform;
        }
    }
    IK_CORE_ASSERT(false, "Could not find uniform !!!");
    return nullptr;
}

/// Get the uniform buffer from Material
/// @param uniformDeclaration uniform declaration in shader
Buffer& Material::GetUniformBufferTarget(ShaderUniformDeclaration* uniformDeclaration) {
    switch (uniformDeclaration->GetDomain()) {
        case ShaderDomain::Vertex:    return m_VSUniformStorageBuffer;
        case ShaderDomain::Fragment:  return m_FSUniformStorageBuffer;
        case ShaderDomain::Geometry:  return m_GSUniformStorageBuffer;
        case ShaderDomain::None:
        default: IK_CORE_ASSERT(false, "Invalid uniform declaration domain! Material does not support this shader type.");
    }
}

/// Find resource location
/// @param name name of resoulce uniform
ShaderResourceDeclaration* Material::FindResourceDeclaration(const std::string& name) {
    auto& resources = m_Shader->GetResources();
    for (ShaderResourceDeclaration* resource : resources) {
        if (resource->GetName() == name)
            return resource;
    }
    IK_CORE_ASSERT(false, "Could not find texure uniform");
    return nullptr;
}

/// Update Material instance values for uniform
/// @param decl uniform declaretion
void Material::OnMaterialValueUpdated(ShaderUniformDeclaration* decl) {
    for (auto mi : m_MaterialInstances)
        mi->OnMaterialValueUpdated(decl);
}

/// Upload the texture to the materual
/// @param name name of Shdaer uniform
/// @param texture Texture Materual
void Material::Set(const std::string& name, const std::shared_ptr<Texture>& texture) {
    Renderer::Submit([this, name, texture]() {
        auto decl = FindResourceDeclaration(name);
    
        uint32_t slot = decl->GetRegister();
        if (m_Textures.size() <= slot)
            m_Textures.resize((size_t)slot + 1);
        m_Textures[slot] = texture;
    });
}

/// Bind the material and shader
void Material::Bind() {
    m_Shader->Bind();
    
    if (m_VSUniformStorageBuffer)
        m_Shader->SetVSMaterialUniformBuffer(m_VSUniformStorageBuffer);
    
    if (m_FSUniformStorageBuffer)
        m_Shader->SetFSMaterialUniformBuffer(m_FSUniformStorageBuffer);
    
    if (m_GSUniformStorageBuffer)
        m_Shader->SetGSMaterialUniformBuffer(m_GSUniformStorageBuffer);
    
    BindTextures();
}

/// Unbind the material
void Material::Unbind() {
    m_Shader->Unbind();
    UnbindTextures();
}

/// Bind all the texture in material
void Material::BindTextures() {
    for (size_t i = 0; i < m_Textures.size(); i++) {
        auto& texture = m_Textures[i];
        if (texture)
            texture->Bind((uint32_t)i);
    }
}

/// Unbind All the textures
void Material::UnbindTextures() {
    for (size_t i = 0; i < m_Textures.size(); i++) {
        auto& texture = m_Textures[i];
        if (texture)
            texture->Unbind();
    }
}

/// Return the containing shader
const std::shared_ptr<Shader> Material::GetShader() const {
    return m_Shader;
}

/// Create instance of Materia instance
/// @param material Material with bind
std::shared_ptr<MaterialInstance> MaterialInstance::Create(const std::shared_ptr<Material>& material, const std::string& name) {
    return std::make_shared<MaterialInstance>(material, name);
}

/// Materialinstance constructor
/// @param material Material
/// @param name Name
MaterialInstance::MaterialInstance(const std::shared_ptr<Material>& material, const std::string& name) : m_Material(material), m_Name(name) {
    Renderer::Submit([this]() {
        IK_LOG_SEPARATOR();
        IK_CORE_INFO("Creating MaterialInstance ...");
        IK_CORE_INFO("    Name               : {0}", m_Name);
        IK_CORE_INFO("    Shader             : {0}", m_Material->m_Shader->GetName());
        m_Material->m_MaterialInstances.insert(this);
        AllocateStorage();
    });
}

/// Material instnace destructor
MaterialInstance::~MaterialInstance() {
    IK_LOG_SEPARATOR();
    IK_CORE_WARN("Destroying MaterialInstance ...");
    IK_CORE_WARN("    Name   : {0}", m_Name);
    IK_CORE_WARN("    Shader : {0}", m_Material->m_Shader->GetName());
    
    m_Material->m_MaterialInstances.erase(this);
}

/// Allocate memory to store shader data
void MaterialInstance::AllocateStorage() {
    if (m_Material->m_Shader->HasVSMaterialUniformBuffer()) {
        const auto& vsBuffer = m_Material->m_Shader->GetVSMaterialUniformBuffer();
        m_VSUniformStorageBuffer.Allocate(vsBuffer.GetSize());
        memcpy(m_VSUniformStorageBuffer.Data, m_Material->m_VSUniformStorageBuffer.Data, vsBuffer.GetSize());
        
        IK_CORE_INFO("    Vertex Buffer      : {0} Bytes", vsBuffer.GetSize());
    }
    
    if (m_Material->m_Shader->HasFSMaterialUniformBuffer()) {
        const auto& psBuffer = m_Material->m_Shader->GetFSMaterialUniformBuffer();
        m_FSUniformStorageBuffer.Allocate(psBuffer.GetSize());
        memcpy(m_FSUniformStorageBuffer.Data, m_Material->m_FSUniformStorageBuffer.Data, psBuffer.GetSize());
        
        IK_CORE_INFO("    Fragment Buffer    : {0} Bytes", psBuffer.GetSize());
    }
    
    if (m_Material->m_Shader->HasGSMaterialUniformBuffer()) {
        const auto& gsBuffer = m_Material->m_Shader->GetGSMaterialUniformBuffer();
        m_GSUniformStorageBuffer.Allocate(gsBuffer.GetSize());
        memcpy(m_GSUniformStorageBuffer.Data, m_Material->m_GSUniformStorageBuffer.Data, gsBuffer.GetSize());
        IK_CORE_INFO("    Geomatry Buffer    : {0} Bytes", gsBuffer.GetSize());
    }
}

/// Update Material
/// @param decl Unifrom decl ptr
void MaterialInstance::OnMaterialValueUpdated(ShaderUniformDeclaration* decl) {
    if (m_OverriddenValues.find(decl->GetName()) == m_OverriddenValues.end()) {
        auto& buffer = GetUniformBufferTarget(decl);
        auto& materialBuffer = m_Material->GetUniformBufferTarget(decl);
        buffer.Write(materialBuffer.Data + decl->GetOffset(), decl->GetSize(), decl->GetOffset());
    }
}

/// Get uniform buffer for uniform
/// @param uniformDeclaration uniform ptr
Buffer& MaterialInstance::GetUniformBufferTarget(ShaderUniformDeclaration* uniformDeclaration) {
    switch (uniformDeclaration->GetDomain()) {
        case ShaderDomain::Vertex:    return m_VSUniformStorageBuffer;
        case ShaderDomain::Fragment:  return m_FSUniformStorageBuffer;
        case ShaderDomain::Geometry:  return m_GSUniformStorageBuffer;
        case ShaderDomain::None:
        default: IK_CORE_ASSERT(false, "Invalid uniform declaration domain! Material does not support this shader type.");
    }
}

/// Upload the texture to material instance buffer
/// @param name name of uniform
/// @param texture Texture shared ptr
void MaterialInstance::Set(const std::string& name, const std::shared_ptr<Texture>& texture) {
    Renderer::Submit([this, name, texture]() {
        auto decl = m_Material->FindResourceDeclaration(name);
        
        uint32_t slot = decl->GetRegister();
        if (m_Textures.size() <= slot)
            m_Textures.resize((size_t)slot + 1);
        m_Textures[slot] = texture;
    });
}

/// Bind Material intance
void MaterialInstance::Bind() {
    m_Material->m_Shader->Bind();
    m_Material->BindTextures();
    
    if (m_VSUniformStorageBuffer)
        m_Material->m_Shader->SetVSMaterialUniformBuffer(m_VSUniformStorageBuffer);
    
    if (m_FSUniformStorageBuffer)
        m_Material->m_Shader->SetFSMaterialUniformBuffer(m_FSUniformStorageBuffer);
    
    if (m_GSUniformStorageBuffer)
        m_Material->m_Shader->SetGSMaterialUniformBuffer(m_GSUniformStorageBuffer);
    
    for (size_t i = 0; i < m_Textures.size(); i++) {
        auto& texture = m_Textures[i];
        if (texture)
            texture->Bind((uint32_t)i);
    }
}

/// Unbind the material
void MaterialInstance::Unbind() {
    m_Material->m_Shader->Unbind();
    m_Material->UnbindTextures();
    
    for (size_t i = 0; i < m_Textures.size(); i++) {
        auto& texture = m_Textures[i];
        if (texture)
            texture->Unbind();
    }
}

const std::string& MaterialInstance::GetName() const { return m_Name; }
void MaterialInstance::SetName(const std::string& name) { m_Name = name; }
