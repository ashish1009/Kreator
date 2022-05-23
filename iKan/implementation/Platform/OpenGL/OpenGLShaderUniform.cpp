//
//  OpenGLShaderUniform.cpp
//  iKan
//
//  Created by iKan on 23/05/22.
//

#include "OpenGLShaderUniform.hpp"

using namespace iKan;

/// Convert Type from string to enum (Resources types)
/// @param type type of uniform sampler in string
OpenGLShaderResourceDeclaration::Type OpenGLShaderResourceDeclaration::StringToType(const std::string& type) {
    if (type == "sampler2D")    return Type::TEXTURE2D;
    if (type == "sampler2DMS")  return Type::TEXTURE2D;
    if (type == "samplerCube")  return Type::TEXTURECUBE;
    
    return Type::NONE;
}

/// Convert Type from enum to string(Resources types)
/// @param type type uniform sampler
std::string OpenGLShaderResourceDeclaration::TypeToString(Type type) {
    switch (type) {
        case Type::NONE:        return "None";
        case Type::TEXTURE2D:   return "sampler2D";
        case Type::TEXTURECUBE: return "samplerCube";
    }
    return "Invalid Type";
}

/// OpenGLShaderResourceDeclaration Constructor
/// @param type Shader type
/// @param name unifrom name
/// @param count count of uniform
OpenGLShaderResourceDeclaration::OpenGLShaderResourceDeclaration(Type type, const std::string& name, uint32_t count)
: m_Type(type), m_Name(name), m_Count(count) {
    IK_CORE_INFO("            {0} {1} [{2}]", TypeToString(m_Type), m_Name, m_Count);
}

/// Get the size of type in bytes
/// @param type uniform variable type
uint32_t OpenGLShaderUniformDeclaration::SizeOfUniformType(Type type) {
    switch (type) {
        case OpenGLShaderUniformDeclaration::Type::NONE:       return 0;
        case OpenGLShaderUniformDeclaration::Type::BOOLEAN:    return 4;
        case OpenGLShaderUniformDeclaration::Type::INT32:      return 4;
        case OpenGLShaderUniformDeclaration::Type::FLOAT32:    return 4;
        case OpenGLShaderUniformDeclaration::Type::VEC2:       return 4 * 2;
        case OpenGLShaderUniformDeclaration::Type::VEC3:       return 4 * 3;
        case OpenGLShaderUniformDeclaration::Type::VEC4:       return 4 * 4;
        case OpenGLShaderUniformDeclaration::Type::MAT3:       return 4 * 3 * 3;
        case OpenGLShaderUniformDeclaration::Type::MAT4:       return 4 * 4 * 4;
        case OpenGLShaderUniformDeclaration::Type::STRUCT:     IK_CORE_ASSERT(false, "No need to return the size in case of struct")
    }
    return 0;
}

/// Convert Type from string to enum (fundamenta types). None for non fundamental like structures
/// @param type uniform variable type in string
OpenGLShaderUniformDeclaration::Type OpenGLShaderUniformDeclaration::StringToType(const std::string& type) {
    if (type == "bool")     return Type::BOOLEAN;
    if (type == "int")      return Type::INT32;
    if (type == "float")    return Type::FLOAT32;
    if (type == "vec2")     return Type::VEC2;
    if (type == "vec3")     return Type::VEC3;
    if (type == "vec4")     return Type::VEC4;
    if (type == "mat3")     return Type::MAT3;
    if (type == "mat4")     return Type::MAT4;
    
    return Type::NONE;
}

/// Convert Type from enum to string(fundamenta types)
/// @param type uniform variable type
std::string OpenGLShaderUniformDeclaration::TypeToString(Type type) {
    switch (type) {
        case OpenGLShaderUniformDeclaration::Type::NONE:       return "None";
        case OpenGLShaderUniformDeclaration::Type::BOOLEAN:    return "bool";
        case OpenGLShaderUniformDeclaration::Type::INT32:      return "int32";
        case OpenGLShaderUniformDeclaration::Type::FLOAT32:    return "float";
        case OpenGLShaderUniformDeclaration::Type::VEC2:       return "vec2";
        case OpenGLShaderUniformDeclaration::Type::VEC3:       return "vec3";
        case OpenGLShaderUniformDeclaration::Type::VEC4:       return "vec4";
        case OpenGLShaderUniformDeclaration::Type::MAT3:       return "mat3";
        case OpenGLShaderUniformDeclaration::Type::MAT4:       return "mat4";
        case OpenGLShaderUniformDeclaration::Type::STRUCT:     return "struct";
    }
    return "Invalid Type";
}

/// OpenGLShaderUniformDeclaration Constructor
/// @param domain shader domain type
/// @param uniformStruct structure stored from shader
/// @param name nsme of uniform
/// @param count count if array
OpenGLShaderUniformDeclaration::OpenGLShaderUniformDeclaration(ShaderDomain domain, ShaderStruct* uniformStruct, const std::string& name, uint32_t count)
: m_Struct(uniformStruct), m_Type(OpenGLShaderUniformDeclaration::Type::STRUCT), m_Domain(domain), m_Name(name), m_Count(count) {
    m_Size = m_Struct->GetSize() * count;
    IK_CORE_INFO("            {0} {1} [{2}] (Size : {3}) ", TypeToString(m_Type), m_Name, m_Count, m_Size);
}

/// OpenGLShaderUniformDeclaration Constructor with type
/// @param domain shader domain type
/// @param type unifom type (non structure)
/// @param name nsme of uniform
/// @param count count if array
OpenGLShaderUniformDeclaration::OpenGLShaderUniformDeclaration(ShaderDomain domain, Type type, const std::string& name, uint32_t count)
: m_Type(type), m_Struct(nullptr), m_Domain(domain), m_Name(name), m_Count(count) {
    m_Size = SizeOfUniformType(m_Type) * m_Count;
    IK_CORE_INFO("            {0} {1} [{2}] (Size : {3}) ", TypeToString(m_Type), m_Name, m_Count, m_Size);
}

/// Update the offset value
/// @param offset offset of uniform
void OpenGLShaderUniformDeclaration::SetOffset(uint32_t offset) {
    if (m_Type == OpenGLShaderUniformDeclaration::Type::STRUCT)
        m_Struct->SetOffset(offset);
    
    m_Offset = offset;
}

/// OpenGLShaderUniformBufferDeclaration Constructor
/// @param name nsme of uniform
/// @param domain shader domain
OpenGLShaderUniformBufferDeclaration::OpenGLShaderUniformBufferDeclaration(const std::string& name, ShaderDomain domain)
: m_Name(name), m_Domain(domain), m_Size(0), m_Register(0) { }

/// OpenGLShaderUniformBufferDeclaration destructor
OpenGLShaderUniformBufferDeclaration::~OpenGLShaderUniformBufferDeclaration() {
    for (auto* uniform : m_Uniforms) {
        delete uniform;
        uniform = nullptr;
    }
    
    m_Uniforms.clear();
}

/// Push the uniform in vector
/// @param uniform Uniform of shader
void OpenGLShaderUniformBufferDeclaration::PushUniform(OpenGLShaderUniformDeclaration* uniform) {
    uint32_t offset = 0;
    if (m_Uniforms.size()) {
        OpenGLShaderUniformDeclaration* previous = (OpenGLShaderUniformDeclaration*)m_Uniforms.back();
        offset = previous->m_Offset + previous->m_Size;
    }
    uniform->SetOffset(offset);
    m_Size += uniform->GetSize();
    m_Uniforms.push_back(uniform);
}

/// Find the uniform from vector
/// @param name nsme of uniform
ShaderUniformDeclaration* OpenGLShaderUniformBufferDeclaration::FindUniform(const std::string& name) {
    for (ShaderUniformDeclaration* uniform : m_Uniforms) {
        if (uniform->GetName() == name)
            return uniform;
    }
    return nullptr;
}
