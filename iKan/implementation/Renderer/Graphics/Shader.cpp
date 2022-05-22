//
//  Shader.cpp
//  iKan
//
//  Created by iKan on 21/05/22.
//

#include "Shader.hpp"
#include "Platform/OpenGL/OpenGLShader.hpp"

using namespace iKan;

/// Create instance of Shader depending on current Renderer API
/// @param path Shader file glsl path
std::shared_ptr<Shader> Shader::Create(const std::string &path) {
    switch (Renderer::GetAPI()) {
        case Renderer::API::OpenGL: return std::make_shared<OpenGLShader>(path);
        case Renderer::API::None:
        default: IK_CORE_ASSERT(false, "Invalid Renderer API (None)"); break;
    }
}
