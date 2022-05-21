//
//  Renderer.cpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#include "Renderer.hpp"
#include "Renderer/Utils/RendererAPI.hpp"
#include "Renderer/Utils/BatchRenderer.hpp"

using namespace iKan;

Renderer::API Renderer::s_API = Renderer::API::None;
std::unique_ptr<RendererAPI> Renderer::s_RendererAPI;

// ----------------------- Core Renderer API ----------------------------------
/// Initialize the Engine Renderer
void Renderer::Init() {
    PROFILE();
    IK_CORE_INFO("Initialising all Renderers");
    if (!s_RendererAPI) {
        // Creating Default Open GL Renderer API instance
        IK_CORE_WARN("    No Renderer API is set before initializing Renderer So Setting Renderer API as 'Open GL'");
        SetAPI(API::OpenGL);
    }
    s_RendererAPI->Init();
    BatchRenderer::Init();
}

/// Shutdown the Engine Renderer Renderer
void Renderer::Shutdown() {
    PROFILE();
    IK_CORE_WARN("Shutting down all Renderers");
    if (s_RendererAPI) {
        s_RendererAPI->Shutdown();
        s_RendererAPI.reset();
    }
    BatchRenderer::Shutdown();
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
    s_RendererAPI = RendererAPI::Create();
}

// ----------------------- Fundamental Renderer API ----------------------------------
/// Clear the renderer Pixel and set Background Color
/// @param vec4 Color to be set as background
void Renderer::Clear(const glm::vec4& color) {
    s_RendererAPI->SetClearColor(color);
    s_RendererAPI->ClearBits();
}

/// Set Background Color
/// @param vec4 Color to be set as background
void Renderer::SetClearColor(const glm::vec4& color) {
    s_RendererAPI->SetClearColor(color);
}

/// Clear the each bits of renderer Pixels
void Renderer::ClearBits() {
    s_RendererAPI->ClearBits();
}

/// Enable or disable Depth Field
/// @param state flag to be set as Depth
void Renderer::Depth(bool state) { s_RendererAPI->Depth(state); }
/// Enable or disable Blend Field
/// @param state flag to be set as Blend
void Renderer::Blend(bool state) { s_RendererAPI->Blend(state); }
/// Enable or disable MultiSample Field
/// @param state flag to be set as MultiSample
void Renderer::MultiSample(bool state) { s_RendererAPI->MultiSample(state); }
