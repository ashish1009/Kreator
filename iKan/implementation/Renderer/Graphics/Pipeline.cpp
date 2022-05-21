//
//  Pipeline.cpp
//  iKan
//
//  Created by iKan on 21/05/22.
//

#include "Pipeline.hpp"
#include "Platform/OpenGL/OpenGLRendererPipeline.hpp"
#include "Renderer/Utils/Renderer.hpp"

using namespace iKan;

/// Create Instance of Pipeline according to enabled Platform
std::shared_ptr<Pipeline> Pipeline::Create() {
    switch (Renderer::GetAPI()) {
        case Renderer::API::OpenGL: return std::make_shared<OpenGLRendererPipeline>();
        case Renderer::API::None:
        default: IK_CORE_ASSERT(false, "Invalid Renderer API (None)"); break;
    }
}
