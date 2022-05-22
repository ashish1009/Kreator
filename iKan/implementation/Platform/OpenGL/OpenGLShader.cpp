//
//  OpenGLShader.cpp
//  iKan
//
//  Created by iKan on 21/05/22.
//

#include "OpenGLShader.hpp"

using namespace iKan;

/// Open GL Shader Constructor
/// @param path Shaderglsl File path
OpenGLShader::OpenGLShader(const std::string& path) : m_AssetPath(path), m_Name(StringUtils::GetNameFromFilePath(path)) {
    PROFILE();
    IK_CORE_INFO("Creating Open GL Shader from File path : '{0}' ...", m_AssetPath);
}

/// Open GL Shader Destructor
OpenGLShader::~OpenGLShader() {
    PROFILE();
    IK_CORE_WARN("Destroying Open GL Shader from File path : '{0}' !!! ", m_AssetPath);
}
