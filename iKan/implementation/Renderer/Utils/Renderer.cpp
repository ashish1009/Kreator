//
//  Renderer.cpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#include "Renderer.hpp"
#include "Renderer/Utils/RendererAPI.hpp"
#include "Renderer/Utils/BatchRenderer.hpp"
#include "Renderer/Utils/RendererStats.hpp"
#include "Renderer/Utils/ShaderLibrary.hpp"
#include "Renderer/Utils/TextureLibrary.hpp"
#include "Renderer/Graphics/Pipeline.hpp"

using namespace iKan;

Renderer::API Renderer::s_API = Renderer::API::None;
std::unique_ptr<RendererAPI> Renderer::s_RendererAPI;
std::set<RendererID> Renderer::s_TextureRendererIDs;

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
    PropertyGrid::Init();
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
    
    // Destorry the Shader library and Texture
    ShaderLibrary::ResetShaders();
    TextureLibrary::ResetTextures();
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
    IK_CORE_INFO("Setting Renderer API : '{0}'", rendererAPI.c_str());
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

/// Update the Graphics Viewport siz
/// @param width new width
/// @param height new heughr
void Renderer::SetViewportSize(float width, float height) {
    s_RendererAPI->SetViewPortSize(width, height);
}

/// Get the Pixel ID from Viewport
/// @param mx x Pixel of Viewport
/// @param my y Pizel of Viewport
/// @param pixelData Pizel Value returned
void Renderer::GetEntityIdFromPixels(int32_t mx, int32_t my, int32_t& pixelData) {
    s_RendererAPI->GetEntityIdFromPixels(mx, my, pixelData);
}

/// Draw Vertex Array with indexed cound
/// @param pipeline vertex array ref pointer
/// @param count count
void Renderer::DrawIndexed(const std::shared_ptr<Pipeline>& pipeline, uint32_t count) {
    s_RendererAPI->DrawIndexed(pipeline, count);
}

/// Render Complex submesh
/// @param pipeline Pipeline
/// @param count Count of index in submesh
/// @param indicesdata indices data
/// @param basevertex Base vertex pos
void Renderer::DrawIndexedBaseVertex(const std::shared_ptr<Pipeline>& pipeline, uint32_t count, void* indicesdata, uint32_t basevertex) {
    s_RendererAPI->DrawIndexedBaseVertex(pipeline, count, indicesdata, basevertex);
}

// --------------------- Renderer Stats API ---------------------------------
/// Restet the renderer Stats each frame
/// NOTE: Only those stats will be reset that need to be reset each frame
/// (Draw Calls, Index Count, Vertex Count)
void Renderer::ResetStatsEachFrame() {
    RendererStatistics::Get().ResetEachFrame();
}

/// Reset all the stats at all
void Renderer::ResetStats() {
    RendererStatistics::Get().ResetAll();
}

/// Render all the stats in Imgui
void Renderer::ImguiRendererStats() {
    RendererStatistics::Get().ImguiRendererStats();
}

// ---------------- Texture Renderer API Manager ----------------------------
/// Check is this renderer ID already given to some texture
/// @param rendererId Renderer ID to be checked
bool Renderer::IsTextureRendererIDExist(RendererID rendererId) {
    return std::find(s_TextureRendererIDs.begin(), s_TextureRendererIDs.end(), rendererId) != s_TextureRendererIDs.end();
}

/// Add Texture renderer ID so that they wont get repeated
/// @param rendererId Renderer ID to be added in set
void Renderer::AddRendererIDs(RendererID rendererId) {
    // Extra Check
    IK_CORE_ASSERT(std::find(s_TextureRendererIDs.begin(), s_TextureRendererIDs.end(), rendererId) == s_TextureRendererIDs.end(), "Repeated Renderer ID... ");
    s_TextureRendererIDs.insert(rendererId);
}

/// Remove Texture renderer ID so that they wont get repeated
/// @param rendererId Renderer ID to be added in set
void Renderer::RemoveRendererIDs(RendererID rendererId) {
    s_TextureRendererIDs.erase(rendererId);
}

// ---------------- Shader Manager ----------------------------
/// Generate and store a new shader if not created already. Else return already created Shader
/// @param path Shader file path
std::shared_ptr<Shader> Renderer::GetShader(const std::string& path) {
    return ShaderLibrary::GetShader(path);
}

// ---------------- Texture Manager ----------------------------
/// Generate and store a new Texture if not created already. Else return already created Texture
/// @param path Texture file path
std::shared_ptr<Texture> Renderer::GetTexture(const std::string& path) {
    return TextureLibrary::GetTexture(path);
}
