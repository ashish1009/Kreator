//
//  OpenGLRendererPipeline.cpp
//  iKan
//
//  Created by iKan on 21/05/22.
//

#include "OpenGLRendererPipeline.hpp"

using namespace iKan;

/// Open GL Vertex Array Constructor
OpenGLRendererPipeline::OpenGLRendererPipeline() {
    PROFILE();
    IK_CORE_INFO("Creating Open GL Vertex Array ... ");
}

/// Open GL Vertex Array Destructor
OpenGLRendererPipeline::~OpenGLRendererPipeline() {
    PROFILE();
    IK_CORE_WARN("Destroying Open GL Vertex Array !!!");
}
