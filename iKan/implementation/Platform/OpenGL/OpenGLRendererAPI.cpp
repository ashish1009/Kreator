//
//  OpenGLRendererAPI.cpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#include "OpenGLRendererAPI.hpp"
#include "Renderer/Graphics/Pipeline.hpp"
#include "Renderer/Utils/RendererStats.hpp"
#include <glad/glad.h>

using namespace iKan;

/// Open GL Renderer API Constructor
OpenGLRendererAPI::OpenGLRendererAPI() {
    IK_CORE_INFO("Creating Open GL Renderer API ...");
}

/// Open GL Renderer API Constructor
OpenGLRendererAPI::~OpenGLRendererAPI() {
    IK_CORE_WARN("Destroying Open GL Renderer API !!!");
}

/// Initialize the Renderer API
void OpenGLRendererAPI::Init() const {
    PROFILE();

    IK_LOG_SEPARATOR();
    IK_CORE_INFO("Initializeing Open GL Renderer API");
    MultiSample(true);
    Depth(true);
    Blend(true);
    IK_LOG_SEPARATOR();
    
    auto& caps = RendererAPI::Capabilities::Get();
    
    caps.Vendor   = (const char*)glGetString(GL_VENDOR);
    caps.Renderer = (const char*)glGetString(GL_RENDERER);
    caps.Version  = (const char*)glGetString(GL_VERSION);
}

/// Shut down the Open GL Renderer API
void OpenGLRendererAPI::Shutdown() const {
    IK_CORE_WARN("Shutting down Open GL Renderer API");
}

/// Update the Depth field. Enable or Disablex
void OpenGLRendererAPI::Depth(bool state) const {
    IK_CORE_INFO("    Open GL Depth Feild : {0}", state);
    if (state)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
}

/// Update the Blend field. Enable or Disablex
void OpenGLRendererAPI::Blend(bool state) const {
    IK_CORE_INFO("    Open GL Blend Feild : {0}", state);
    if (state) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else {
        glDisable(GL_BLEND);
    }
}

/// Update the Multisample field. Enable or Disablex
void OpenGLRendererAPI::MultiSample(bool state) const {
    IK_CORE_INFO("    Open GL Multi Sample Feild : {0}", state);
    if (state)
        glEnable(GL_MULTISAMPLE);
    else
        glDisable(GL_MULTISAMPLE);
}

/// Set Background Color
/// @param vec4 Color to be set as background
void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) const {
    glClearColor(color.r, color.g, color.b, color.a);
}

/// Clear the each bits of renderer Pixels
void OpenGLRendererAPI::ClearBits() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

/// Draw Indexed Vertex Array
/// @param pipeline pipeline having vertex buffer and index buffer
/// @param count number of Indices (if 0 then use index buffer of Vertex array)
void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<Pipeline>& pipeline, uint32_t count) const {
    pipeline->Bind();
    uint32_t c = (count ? count : pipeline->GetIndexBuffer()->GetCount());
    glDrawElements(GL_TRIANGLES, c , GL_UNSIGNED_INT, nullptr);
    
    // Unbinding Textures and va
    glBindTexture(GL_TEXTURE_2D, 0);
    pipeline->Unbind();
    
    RendererStatistics::Get().DrawCalls++;
}
