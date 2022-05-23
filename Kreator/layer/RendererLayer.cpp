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
    PROFILE();
    IK_INFO("Attaching {0} Layer to Application", m_Name);
    
    FrameBuffer::Specification spec;
    spec.Attachments = {
        FrameBuffer::Attachment::TextureFormat::RGBA8,
        FrameBuffer::Attachment::TextureFormat::Depth24Stencil,
        FrameBuffer::Attachment::TextureFormat::R32I
    };
    m_VpData.FrameBuffer = FrameBuffer::Create(spec);
}

/// Renderer Layer Detach
void RendererLayer::Detach() {
    PROFILE();
    IK_WARN("Detaching {0} Layer to Application", m_Name);
}

/// Update Renderer Layer each frame
/// @param ts Time step between 2 Frames
void RendererLayer::Update(Timestep ts) {
    Renderer::ResetStatsEachFrame();
    
    m_VpData.FrameBuffer->Bind();
    {
        Renderer::Clear({ 0.2f, 0.2f, 0.2f, 1.0f });
    }
    m_VpData.FrameBuffer->Unbind();
}

/// Render GUI Window each frame for Renderer Layer
void RendererLayer::RenderGui() {
    ImguiAPI::StartDcocking();
    
    ImguiAPI::FrameRate();
    Renderer::ImguiRendererStats();
    
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Kreator Viewport");
        {
            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
            size_t textureID = m_VpData.FrameBuffer->GetColorAttachmentIds()[0];
            ImGui::Image((void*)textureID, viewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        }
        
        ImGui::End(); // ImGui::Begin("Kreator Viewport");
        ImGui::PopStyleVar(); // ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
    }
    
    ImguiAPI::EndDcocking();
}

/// Handle Event interupt for Renderer Layer
/// @param event Event base class Instance.
void RendererLayer::EventHandler(Event& event) {

}
