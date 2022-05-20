//
//  RendererAPI.cpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#include "RendererAPI.hpp"
#include "Renderer/Utils/Renderer.hpp"
#include "Platform/OpenGL/OpenGLRendererAPI.hpp"

using namespace iKan;

/// Create instance of Renderer API based on the supported API
std::unique_ptr<RendererAPI> RendererAPI::Create() {
    switch (Renderer::GetAPI()) {
        case Renderer::API::OpenGL : return std::make_unique<OpenGLRendererAPI>();
        case Renderer::API::None:
        default: IK_CORE_ASSERT(false, "Render API not Supporting");
    }
}
