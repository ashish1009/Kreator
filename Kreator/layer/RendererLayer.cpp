//
//  RendererLayer.cpp
//  Kreator
//
//  Created by iKan on 19/05/22.
//

#include "RendererLayer.hpp"

/// Renderer Layer Constructor
RendererLayer::RendererLayer() : Layer("Renderer") {
    IK_INFO("Creating {0} Layer ...", m_Name);
}

/// Renderer layer Destructor
RendererLayer::~RendererLayer() {
    IK_WARN("Creating {0} Layer !!!", m_Name);
}

/// Renderer Layer Attach
void RendererLayer::Attach() {
    IK_INFO("Attaching {0} Layer to Application", m_Name);
}

/// Renderer Layer Detach
void RendererLayer::Detach() {
    IK_WARN("Detaching {0} Layer to Application", m_Name);
}

/// Update Renderer Layer each frame
void RendererLayer::Update() {
    
}

/// Render GUI Window each frame for Renderer Layer
void RendererLayer::RenderGui() {

}

/// Handle Event interupt for Renderer Layer
/// @param event Event base class Instance.
void RendererLayer::EventHandler(Event& event) {

}
