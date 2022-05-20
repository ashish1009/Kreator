//
//  OpenGLRendererAPI.cpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#include "OpenGLRendererAPI.hpp"
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
    IK_CORE_INFO("Initializeing Open GL Renderer API");

    glEnable(GL_MULTISAMPLE);

    Depth(true);
    Blend(true);
    
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
    if (state) {
        IK_CORE_INFO("Eanble Open GL Depth Feild");
        glEnable(GL_DEPTH_TEST);
    }
    else {
        IK_CORE_INFO("Disable Open GL Depth Feild");
        glDisable(GL_DEPTH_TEST);
    }
}

/// Update the Blend field. Enable or Disablex
void OpenGLRendererAPI::Blend(bool state) const {
    if (state) {
        IK_CORE_INFO("Eanble Open GL Blend Feild");
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else {
        IK_CORE_INFO("Disable Open GL Blend Feild");
        glDisable(GL_BLEND);
    }
}
