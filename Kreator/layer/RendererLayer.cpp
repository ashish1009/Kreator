//
//  RendererLayer.cpp
//  Kreator
//
//  Created by iKan on 19/05/22.
//

#include "RendererLayer.hpp"

glm::vec3 CameraTranslation = { 0.0f, 0.0f, 0.1f };
glm::vec3 QuadTranslation = { 0.0f, 0.0f, 0.1f };

/// Renderer Layer Constructor
RendererLayer::RendererLayer() : Layer("Renderer") {
    IK_INFO("Creating {0} Layer ...", m_Name);
}

/// Renderer layer Destructor
RendererLayer::~RendererLayer() {
    IK_WARN("Destroying {0} Layer !!!", m_Name);
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
    
    // Temp TODO: Create Scene using UI
    // Create scene
    m_ActiveScene = Scene::Create();
    m_SHP = SceneHierarchyPannel::Create(m_ActiveScene);
    
    // Temp Init
    m_SceneCamera = SceneCamera::Create(SceneCamera::ProjectionType::Orthographic);
    m_EditorCamera = EditorCamera::Create();
}

/// Renderer Layer Detach
void RendererLayer::Detach() {
    PROFILE();
    IK_WARN("Detaching {0} Layer to Application", m_Name);
}

/// Update Renderer Layer each frame
/// @param ts Time step between 2 Frames
void RendererLayer::Update(Timestep ts) {
    if (!m_ActiveScene)
        return;
    
    // If resize the window call the update the Scene View port and Frame buffer should be resized
    if (const FrameBuffer::Specification& spec = m_VpData.FrameBuffer->GetSpecification();
        (uint32_t)m_VpData.Size.x > 0 && (uint32_t)m_VpData.Size.y > 0 && // zero sized framebuffer is invalid
        (spec.Width != (uint32_t)m_VpData.Size.x || spec.Height != (uint32_t)m_VpData.Size.y)) {
        m_VpData.FrameBuffer->Resize((uint32_t)m_VpData.Size.x, (uint32_t)m_VpData.Size.y);
        m_ActiveScene->SetViewport((uint32_t)m_VpData.Size.x, (uint32_t)m_VpData.Size.y);
    }

    Renderer::ResetStatsEachFrame();
    
    m_VpData.FrameBuffer->Bind();
    {
        Renderer::Clear({ 0.2f, 0.2f, 0.2f, 1.0f });
        
        m_ActiveScene->Update(ts);
        m_VpData.UpdateMousePos();
    }
    m_VpData.FrameBuffer->Unbind();
}

/// Render GUI Window each frame for Renderer Layer
void RendererLayer::RenderGui() {
    ImguiAPI::StartDcocking();
    
    if (m_ActiveScene)
        m_ActiveScene->RenderImgui();

    // Debug Window
    if (m_ActiveScene->IsEditing()) {
        ImguiAPI::FrameRate();
        Renderer::ImguiRendererStats();
        m_VpData.RenderImgui();
        
        m_SHP->RenderImgui();
    }
        
    // Viewport
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Kreator Viewport");
        ImGui::PushID("Kreator Viewport");
        
        m_VpData.Focused = ImGui::IsWindowFocused();
        m_VpData.Hovered = ImGui::IsWindowHovered();
        
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_VpData.Size = { viewportPanelSize.x, viewportPanelSize.y };
        
        size_t textureID = m_VpData.FrameBuffer->GetColorAttachmentIds()[0];
        PropertyGrid::Image((void*)textureID, { m_VpData.Size.x, m_VpData.Size.y }, { 0, 1 }, { 1, 0 });

        m_VpData.UpdateBound();

        ImGui::PopID();
        ImGui::End(); // ImGui::Begin("Kreator Viewport");
        ImGui::PopStyleVar(); // ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
    }
    
    ImguiAPI::EndDcocking();
}

/// Handle Event interupt for Renderer Layer
/// @param event Event base class Instance.
void RendererLayer::EventHandler(Event& event) {
    if (m_ActiveScene)
        m_ActiveScene->EventHandler(event);
}
