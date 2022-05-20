//
//  Renderer.cpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#include "Renderer.hpp"

using namespace iKan;

Renderer::API Renderer::s_API = Renderer::API::None;

/// Initialize the Engine Renderer
void Renderer::Init() {
}

/// Shutdown the Engine Renderer Renderer
void Renderer::Shutdown() {
}

/// Return the current active Renderer API
Renderer::API Renderer::GetAPI() { return s_API; }

/// Set the Active renderer API and instanciate Renderer API
/// @param api active API
void Renderer::SetAPI(Renderer::API api) {
#ifdef IK_DEBUG_FEATURE
    std::string rendererAPI = "None";
    switch (api) {
        case API::OpenGL : rendererAPI = "Open GL"; break;
        case API::None:
        default:
            IK_CORE_ASSERT(false, "Invalid Renderer API");
    }
    IK_CORE_INFO("Renderer API is set to '{0}'", rendererAPI.c_str());
#endif
    s_API = api;
}

