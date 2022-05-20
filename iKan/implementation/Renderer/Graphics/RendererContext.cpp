//
//  RendererContext.cpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#include "RendererContext.hpp"
#include "Renderer/Utils/Renderer.hpp"
#include "Platform/OpenGL/OpenGLRendererContext.hpp"

using namespace iKan;

/// Create instance of Renderer Context based on the current AP Supported by iKan
/// @param window GLFW Window instance created by Window class
std::unique_ptr<RendererContext> RendererContext::Create(GLFWwindow *window) {
    switch(Renderer::GetAPI()) {
        case Renderer::API::OpenGL: return std::make_unique<OpenGLRendererContext>(window);
        case Renderer::API::None:
        default: IK_CORE_ASSERT(false, "Invalid Renderer API which is not supported by iKan");
    }
}
