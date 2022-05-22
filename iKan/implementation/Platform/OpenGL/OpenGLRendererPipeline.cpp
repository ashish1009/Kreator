//
//  OpenGLRendererPipeline.cpp
//  iKan
//
//  Created by iKan on 21/05/22.
//

#include "OpenGLRendererPipeline.hpp"
#include <glad/glad.h>

using namespace iKan;

/// Open GL Vertex Array Constructor
OpenGLRendererPipeline::OpenGLRendererPipeline() {
    PROFILE();
    glGenVertexArrays(1, &m_RendererID);
    glBindVertexArray(m_RendererID);

    IK_LOG_SEPARATOR();
    IK_CORE_INFO("Creating Open GL Piperline ...");
    IK_CORE_INFO("    Renderer ID : {0}", m_RendererID);
    IK_LOG_SEPARATOR();
}

/// Open GL Vertex Array Destructor
OpenGLRendererPipeline::~OpenGLRendererPipeline() {
    PROFILE();
    IK_CORE_WARN("Destroying Open GL Piperline with Renderer ID : {0} !!!", m_RendererID);
    glDeleteVertexArrays(1, &m_RendererID);
}
