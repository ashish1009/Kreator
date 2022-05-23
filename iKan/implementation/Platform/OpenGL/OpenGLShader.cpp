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

/// Bind current shader to GPU
void OpenGLShader::Bind() const {
    glUseProgram(m_RendererID);
}

/// UnBind current shader to GPU
void OpenGLShader::Unbind() const {
    glUseProgram(0);
};
