//
//  OpenGLShader.cpp
//  iKan
//
//  Created by iKan on 21/05/22.
//

#include "OpenGLShader.hpp"

using namespace iKan;

namespace ShaderUtils {

    /// Get the Open GL Shader Type from string
    /// @param type type of shader in string
    static GLenum ShaderTypeFromString(const std::string& type) {
        if ("vertex" == type)   return GL_VERTEX_SHADER;
        if ("fragment" == type) return GL_FRAGMENT_SHADER;
        if ("geometry" == type) return GL_GEOMETRY_SHADER;
        
        IK_CORE_ASSERT(false, "Unknown shader type!");
    }

#ifdef IK_DEBUG_FEATURE
    /// Get the Open GL Shader Name from Type
    /// @param type type of shader in GL enum
    static const std::string ShaderNameFromType(const GLenum& type) {
        if (type == GL_VERTEX_SHADER) return "Vertex";
        if (type == GL_FRAGMENT_SHADER) return "Fragment";
        if (type == GL_GEOMETRY_SHADER) return "Geomatry";
        
        IK_CORE_ASSERT(false, "Unknown shader type!");
    }
#endif
    
}

/// Open GL Shader Constructor
/// @param path Shaderglsl File path
OpenGLShader::OpenGLShader(const std::string& path) : m_AssetPath(path), m_Name(StringUtils::GetNameFromFilePath(path)) {
    PROFILE();
    m_RendererID = glCreateProgram();

    IK_LOG_SEPARATOR();
    IK_CORE_INFO("Creating Open GL Shader ...");
    IK_CORE_INFO("    Renderer ID : {0} ", m_RendererID);
    IK_CORE_INFO("    Name        : {0} ", m_Name);
    IK_CORE_INFO("    File Path   : {0} ", m_AssetPath);
    IK_CORE_INFO("    ---------------------------------------------");

    PreprocessFile(StringUtils::ReadFromFile(m_AssetPath));
    Compile();
    
    IK_LOG_SEPARATOR();
}

/// Open GL Shader Destructor
OpenGLShader::~OpenGLShader() {
    PROFILE();
    IK_LOG_SEPARATOR();
    IK_CORE_WARN("Destroying Open GL Shader ...");
    IK_CORE_WARN("    Renderer ID : {0} ", m_RendererID);
    IK_CORE_WARN("    Name        : {0} ", m_Name);
    IK_CORE_WARN("    File Path   : {0} ", m_AssetPath);
    IK_LOG_SEPARATOR();
}

/// Process the Shader file and separate the Shaders (Vertex, Fragent and so on) in strings
/// @param source source string where all shader are stored
void OpenGLShader::PreprocessFile(const std::string &source) {
    IK_CORE_INFO("    Processing the Open GL Shader: '{0}'", m_Name.c_str());
    
    const char* token = "#type";
    size_t pos = source.find(token);
    while (pos != std::string::npos) {
        size_t eol = source.find_first_of("\r\n", pos);
        IK_CORE_ASSERT((eol != std::string::npos), "Syntax error");

        size_t begin = pos + strlen(token) + 1;
        std::string type = source.substr(begin, eol - begin);
        IK_CORE_ASSERT(ShaderUtils::ShaderTypeFromString(type), "Invalid shader type specified");

        size_t nextLine = source.find_first_of("\r\n", eol);
        pos = source.find(token, nextLine);

        m_SourceShaderString[ShaderUtils::ShaderTypeFromString(type)] = source.substr(nextLine, pos - (nextLine == std::string::npos ? source.size() - 1 : nextLine));
    }
}

/// Compile the shader code extracted in strings
void OpenGLShader::Compile() {
    IK_CORE_INFO("    Compiling Open GL Shader with File name: '{0}' ", m_Name.c_str());
    
    int32_t glShaderIDIndex = 0;
    std::array<GLuint, MaxShaderSupported> shaderId;
    
    for (const auto& [type, src] : m_SourceShaderString) {
        IK_CORE_INFO("        Compiling '{0}' Shader ", ShaderUtils::ShaderNameFromType(type).c_str());
        
        GLuint shader = glCreateShader(type);
        
        // Attch the shader source and then compile
        const char* string = src.c_str();
        glShaderSource(shader, 1, &string, nullptr);
        glCompileShader(shader);
        
        // Shader Error Handling
        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
            
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
            
            glDeleteShader(shader);
            
            IK_CORE_ERROR("{0}", infoLog.data());
            IK_CORE_ASSERT(false, "Shader compilation failure!");
        }
        // Attach both shader and link them
        glAttachShader(m_RendererID, shader);
        shaderId[glShaderIDIndex++] = shader;
    }
    
    glLinkProgram(m_RendererID);
    // Shader Error Handling
    // Note the different functions here: glGetProgram* instead of glGetShader
    GLint isLinked = 0;
    glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int32_t*)&isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);
        
        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);
        
        // We don't need the program anymore.
        glDeleteProgram(m_RendererID);
        
        for (auto id : shaderId)
            glDeleteShader(id);
        
        IK_CORE_ERROR("{0}", infoLog.data());
        IK_CORE_ASSERT(false, "Shader link failure!");
    }
    
    for (auto id : shaderId)
        glDeleteShader(id);
}

/// Upload the Uniform Vertex shader
/// @param buffer Data buffer stored for vertex Shdaer
void OpenGLShader::SetVSMaterialUniformBuffer(const Buffer& buffer) {
    glUseProgram(m_RendererID);
    ResolveAndSetUniforms(m_VSMaterialUniformBuffer, buffer);
}

/// Upload the Uniform Pixel shader
/// @param buffer Data buffer stored for fragment Shdaer
void OpenGLShader::SetFSMaterialUniformBuffer(const Buffer& buffer) {
    glUseProgram(m_RendererID);
    ResolveAndSetUniforms(m_FSMaterialUniformBuffer, buffer);
}

/// Upload the Uniform Geonatry shader
/// @param buffer Data buffer stored for geomatry Shdaer
void OpenGLShader::SetGSMaterialUniformBuffer(const Buffer& buffer) {
    glUseProgram(m_RendererID);
    ResolveAndSetUniforms(m_GSMaterialUniformBuffer, buffer);
}

/// ResolveAndSetUniforms
/// @param decl Reference pointer for uniform declaretion
/// @param buffer Data buffer stored for specific shader
void OpenGLShader::ResolveAndSetUniforms(const std::shared_ptr<OpenGLShaderUniformBufferDeclaration>& decl, const Buffer& buffer) {
    const std::vector<ShaderUniformDeclaration*>& uniforms = decl->GetUniformDeclarations();
    for (size_t i = 0; i < uniforms.size(); i++) {
        OpenGLShaderUniformDeclaration* uniform = (OpenGLShaderUniformDeclaration*)uniforms[i];
        if (uniform->IsArray())
            ResolveAndSetUniformArray(uniform, buffer);
        else
            ResolveAndSetUniform(uniform, buffer);
    }
}

/// ResolveAndSetUniform
/// @param uniform uniform pointer
/// @param buffer Data buffer stored in shader
void OpenGLShader::ResolveAndSetUniform(OpenGLShaderUniformDeclaration* uniform, const Buffer& buffer) {
    if (uniform->GetLocation() == -1)
        return;
    
    IK_CORE_ASSERT((uniform->GetLocation() != -1), "Uniform has invalid location!");
    
    uint32_t offset = uniform->GetOffset();
    switch (uniform->GetType()) {
        case OpenGLShaderUniformDeclaration::Type::FLOAT32:
            UploadUniformFloat1(uniform->GetLocation(), *(float*)&buffer.Data[offset]);
            break;
        case OpenGLShaderUniformDeclaration::Type::INT32:
        case OpenGLShaderUniformDeclaration::Type::BOOLEAN:
            UploadUniformInt1(uniform->GetLocation(), *(int32_t*)&buffer.Data[offset]);
            break;
        case OpenGLShaderUniformDeclaration::Type::VEC2:
            UploadUniformFloat2(uniform->GetLocation(), *(glm::vec2*)&buffer.Data[offset]);
            break;
        case OpenGLShaderUniformDeclaration::Type::VEC3:
            UploadUniformFloat3(uniform->GetLocation(), *(glm::vec3*)&buffer.Data[offset]);
            break;
        case OpenGLShaderUniformDeclaration::Type::VEC4:
            UploadUniformFloat4(uniform->GetLocation(), *(glm::vec4*)&buffer.Data[offset]);
            break;
        case OpenGLShaderUniformDeclaration::Type::MAT3:
            UploadUniformMat3(uniform->GetLocation(), *(glm::mat3*)&buffer.Data[offset]);
            break;
        case OpenGLShaderUniformDeclaration::Type::MAT4:
            UploadUniformMat4(uniform->GetLocation(), *(glm::mat4*)&buffer.Data[offset]);
            break;
        case OpenGLShaderUniformDeclaration::Type::STRUCT:
            UploadUniformStruct(uniform, buffer.Data, offset);
            break;
        default:
            IK_CORE_ASSERT(false, "Unknown uniform type!");
    }
}

/// ResolveAndSetUniformArray
/// @param uniform Uniform pointer
/// @param buffer Data buffer stored in shader
void OpenGLShader::ResolveAndSetUniformArray(OpenGLShaderUniformDeclaration* uniform, const Buffer& buffer) {
    if (uniform->GetLocation() == -1)
        return;
    
    IK_CORE_ASSERT((uniform->GetLocation() != -1), "Uniform has invalid location!");
    
    uint32_t offset = uniform->GetOffset();
    switch (uniform->GetType()) {
        case OpenGLShaderUniformDeclaration::Type::FLOAT32:
            UploadUniformFloat1(uniform->GetLocation(), *(float*)&buffer.Data[offset]);
            break;
        case OpenGLShaderUniformDeclaration::Type::INT32:
        case OpenGLShaderUniformDeclaration::Type::BOOLEAN:
            UploadUniformInt1(uniform->GetLocation(), *(int32_t*)&buffer.Data[offset]);
            break;
        case OpenGLShaderUniformDeclaration::Type::VEC2:
            UploadUniformFloat2(uniform->GetLocation(), *(glm::vec2*)&buffer.Data[offset]);
            break;
        case OpenGLShaderUniformDeclaration::Type::VEC3:
            UploadUniformFloat3(uniform->GetLocation(), *(glm::vec3*)&buffer.Data[offset]);
            break;
        case OpenGLShaderUniformDeclaration::Type::VEC4:
            UploadUniformFloat4(uniform->GetLocation(), *(glm::vec4*)&buffer.Data[offset]);
            break;
        case OpenGLShaderUniformDeclaration::Type::MAT3:
            UploadUniformMat3(uniform->GetLocation(), *(glm::mat3*)&buffer.Data[offset]);
            break;
        case OpenGLShaderUniformDeclaration::Type::MAT4:
            UploadUniformMat4Array(uniform->GetLocation(), *(glm::mat4*)&buffer.Data[offset], uniform->GetCount());
            break;
        case OpenGLShaderUniformDeclaration::Type::STRUCT:
            UploadUniformStruct(uniform, buffer.Data, offset);
            break;
        default:
            IK_CORE_ASSERT(false, "Unknown uniform type!");
    }
}

/// ResolveAndSetUniformField
/// @param field uniform field
/// @param data data pointer in bytes
/// @param offset offset of this uniform in shader
void OpenGLShader::ResolveAndSetUniformField(const OpenGLShaderUniformDeclaration& field, std::byte* data, int32_t offset) {
    switch (field.GetType()) {
        case OpenGLShaderUniformDeclaration::Type::FLOAT32:
            UploadUniformFloat1(field.GetLocation(), *(float*)&data[offset]);
            break;
        case OpenGLShaderUniformDeclaration::Type::INT32:
        case OpenGLShaderUniformDeclaration::Type::BOOLEAN:
            UploadUniformInt1(field.GetLocation(), *(int32_t*)&data[offset]);
            break;
        case OpenGLShaderUniformDeclaration::Type::VEC2:
            UploadUniformFloat2(field.GetLocation(), *(glm::vec2*)&data[offset]);
            break;
        case OpenGLShaderUniformDeclaration::Type::VEC3:
            UploadUniformFloat3(field.GetLocation(), *(glm::vec3*)&data[offset]);
            break;
        case OpenGLShaderUniformDeclaration::Type::VEC4:
            UploadUniformFloat4(field.GetLocation(), *(glm::vec4*)&data[offset]);
            break;
        case OpenGLShaderUniformDeclaration::Type::MAT3:
            UploadUniformMat3(field.GetLocation(), *(glm::mat3*)&data[offset]);
            break;
        case OpenGLShaderUniformDeclaration::Type::MAT4:
            UploadUniformMat4(field.GetLocation(), *(glm::mat4*)&data[offset]);
            break;
        default:
            IK_CORE_ASSERT(false, "Unknown uniform type!");
    }
}

/// ResolveAndSetUniform
/// @param uniform unfiorm pointer
/// @param buffer data pointer in byted
/// @param offset offset of this uniform in shader
void OpenGLShader::UploadUniformStruct(OpenGLShaderUniformDeclaration* uniform, std::byte* buffer, uint32_t offset) {
    const ShaderStruct& s = uniform->GetShaderUniformStruct();
    const auto& fields = s.GetFields();
    
    for (size_t k = 0; k < fields.size(); k++) {
        OpenGLShaderUniformDeclaration* field = (OpenGLShaderUniformDeclaration*)fields[k];
        ResolveAndSetUniformField(*field, buffer, offset);
        offset += field->m_Size;
    }
}

/// Bind current shader to GPU
void OpenGLShader::Bind() const {
    glUseProgram(m_RendererID);
}

/// UnBind current shader to GPU
void OpenGLShader::Unbind() const {
    glUseProgram(0);
};

/// Description Find the location of Attribute from shader
/// @param name Uniform name
int32_t OpenGLShader::GetUniformLocation(const std::string& name) {
    if (m_LocationMap.find(name) != m_LocationMap.end())
        return m_LocationMap[name];
    
    int32_t location = glGetUniformLocation(m_RendererID, name.c_str());
    if (-1 == location)
        IK_CORE_WARN("Warning: uniform '{0}' doesnt exist", name);
    
    m_LocationMap[name] = location;
    return location;
}

//-------------------------------- Uniforms with name --------------------------------
void OpenGLShader::SetUniformInt1(const std::string& name, int32_t value) {
    glUniform1i(GetUniformLocation(name), value);
}

void OpenGLShader::SetIntArray(const std::string& name, int32_t* values, uint32_t count) {
    int32_t* textureArraySlotData = new int32_t[count];
    memcpy(textureArraySlotData, values, count * sizeof(int32_t));
    glUniform1iv(GetUniformLocation(name), count, textureArraySlotData);
    delete[] textureArraySlotData;
}

void OpenGLShader::SetUniformMat4Array(const std::string& name, const glm::mat4& values, uint32_t count) {
    glUniformMatrix4fv(GetUniformLocation(name), count, GL_FALSE, glm::value_ptr(values));
}

void OpenGLShader::SetUniformMat4(const std::string& name, const glm::mat4& value) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void OpenGLShader::SetUniformMat3(const std::string& name, const glm::mat3& value) {
    glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void OpenGLShader::SetUniformFloat1(const std::string& name, float value) {
    glUniform1f(GetUniformLocation(name), value);
}

void OpenGLShader::SetUniformFloat2(const std::string& name, const glm::vec2& value) {
    glUniform2f(GetUniformLocation(name), value.x, value.y);
}

void OpenGLShader::SetUniformFloat3(const std::string& name, const glm::vec3& value) {
    glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
}

void OpenGLShader::SetUniformFloat4(const std::string& name, const glm::vec4& value) {
    glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
}

//-------------------------------- Uniforms with location --------------------------------
void OpenGLShader::UploadUniformInt1(int32_t location, int32_t value) {
    glUniform1i(location, value);
}

void OpenGLShader::UploadUniformMat4(int32_t location, const glm::mat4& value) {
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void OpenGLShader::UploadUniformMat3(int32_t location, const glm::mat3& value) {
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void OpenGLShader::UploadUniformFloat1(int32_t location, float value) {
    glUniform1f(location, value);
}

void OpenGLShader::UploadUniformFloat2(int32_t location, const glm::vec2& value) {
    glUniform2f(location, value.x, value.y);
}

void OpenGLShader::UploadUniformFloat3(int32_t location, const glm::vec3& value) {
    glUniform3f(location, value.x, value.y, value.z);
}

void OpenGLShader::UploadUniformFloat4(int32_t location, const glm::vec4& value) {
    glUniform4f(location, value.x, value.y, value.z, value.w);
}

void OpenGLShader::UploadUniformMat4Array(uint32_t location, const glm::mat4& values, uint32_t count) {
    glUniformMatrix4fv(location, count, GL_FALSE, glm::value_ptr(values));
}
