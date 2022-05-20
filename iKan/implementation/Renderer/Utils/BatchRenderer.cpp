//
//  BatchRenderer.cpp
//  iKan
//
//  Created by iKan on 20/05/22.
//

#include "BatchRenderer.hpp"

using namespace iKan;

/// Initialize the Batch renderer for 2D Renderer
void BatchRenderer::Init() {
    PROFILE();
    IK_CORE_INFO("Initialising the Batch Renderer 2D ...");
}

/// SHutdown or destroy the batch Renderer
void BatchRenderer::Shutdown() {
    PROFILE();
    IK_CORE_WARN("Shutting down the Batch Renderer 2D !!!");
}
