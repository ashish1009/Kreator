//
//  RendererLayer.cpp
//  Kreator
//
//  Created by iKan on 19/05/22.
//

#include "ChessRendererLayer.hpp"

using namespace Chess;

/// Chess Renderer Layer Constructor
ChessRendererLayer::ChessRendererLayer()
: Layer("Chess Renderer Layer") {
    IK_INFO("Creating '{0}' ...", m_Name);
}

/// Chess Renderer Layer Destructor
ChessRendererLayer::~ChessRendererLayer() {
    IK_WARN("Destroying '{0}' !!!", m_Name);
}

/// Attach the Renderer Layer to the application
void ChessRendererLayer::Attach() {
    IK_INFO("Attaching '{0}'", m_Name);
    
    // Viewport Data setup
    FrameBuffer::Specification spec;
    spec.Width = 2100;
    spec.Height = 900;
    spec.Color = { 0.1, 0.1, 0.1, 1.0 };
    spec.Attachments = {
        FrameBuffer::Attachment::TextureFormat::RGBA8,
        FrameBuffer::Attachment::TextureFormat::R32I,
        FrameBuffer::Attachment::TextureFormat::Depth24Stencil,
    };
    m_ViewportData.FrameBuffer = FrameBuffer::Create(spec);
}

/// Update the renderer Layer each frame
void ChessRendererLayer::Update(Timestep ts) {
    // If resize the window call the update the Scene View port and Frame buffer should be resized
    if (const FrameBuffer::Specification& spec = m_ViewportData.FrameBuffer->GetSpecification();
        (uint32_t)m_ViewportData.Size.x > 0 && (uint32_t)m_ViewportData.Size.y > 0 && // zero sized framebuffer is invalid
        (spec.Width != (uint32_t)m_ViewportData.Size.x || spec.Height != (uint32_t)m_ViewportData.Size.y)) {
        m_ViewportData.FrameBuffer->Resize((uint32_t)m_ViewportData.Size.x, (uint32_t)m_ViewportData.Size.y);
    }
    
    Renderer::ResetStatsEachFrame();

    m_ViewportData.FrameBuffer->Bind();

    Renderer::Clear({ 0.1f, 0.1f, 0.1f, 1.0f });
    m_ViewportData.UpdateMousePos();
    UpdateHoveredEntity();

    m_ViewportData.FrameBuffer->Unbind();
}

/// Render ; for Renderer Layer
void ChessRendererLayer::RenderGui() {
    ImguiAPI::StartDcocking();
    
    // Viewport
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
    ImGui::Begin("Chess Viewport");
    ImGui::PushID("Chess Viewport");
    
    m_ViewportData.Focused = ImGui::IsWindowFocused();
    m_ViewportData.Hovered = ImGui::IsWindowHovered();
    
    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    m_ViewportData.Size = { viewportPanelSize.x, viewportPanelSize.y };
    
    size_t textureID = m_ViewportData.FrameBuffer->GetColorAttachmentIds()[0];
    PropertyGrid::Image((void*)textureID, { m_ViewportData.Size.x, m_ViewportData.Size.y }, { 0, 1 }, { 1, 0 });
    ImGui::PopStyleVar();

    m_ViewportData.UpdateBound();

    ImGui::PopID();
    ImGui::End(); // Viewport
    
    {
        ImguiAPI::FrameRate();
        Renderer::ImguiRendererStats();
        m_ViewportData.RenderImgui();
        
        ImGui::Begin("Debug Window");
        
        
        ImGui::End(); // Debug Window
    }
        
    ImguiAPI::EndDcocking();
}

/// Detach the Renderer Lyer
void ChessRendererLayer::Detach() {
    IK_WARN("Detaching '{0}'", m_Name);
}

/// Handle the Events of Renderer Layer
void ChessRendererLayer::EventHandler(Event& event) {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<KeyPressedEvent>(IK_BIND_EVENT_FN(ChessRendererLayer::OnKeyPressed));
    dispatcher.Dispatch<MouseButtonPressedEvent>(IK_BIND_EVENT_FN(ChessRendererLayer::OnMouseButtonPressed));
    dispatcher.Dispatch<WindowResizeEvent>(IK_BIND_EVENT_FN(ChessRendererLayer::OnWindowResize));
}

/// Mouse button Event
/// @param e Mouse Button event handler
bool ChessRendererLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e) {
    return false;
}

/// Kwy Press Event
/// @param event Key Press event handler
bool ChessRendererLayer::OnKeyPressed(KeyPressedEvent& event) {
    return false;
}

/// Mouse button Event
/// @param e Mouse Button event handler
bool ChessRendererLayer::OnWindowResize(WindowResizeEvent& e) {
    Renderer::SetViewportSize(e.GetWidth(), e.GetHeight());
    return false;
}

/// Update Hovered Entity
void ChessRendererLayer::UpdateHoveredEntity() {
    if (m_ViewportData.Hovered) {
        Renderer::GetEntityIdFromPixels(m_ViewportData.MousePosX, m_ViewportData.MousePosY, m_ViewportData.HoveredEntityID);
//        m_ViewportData.HoveredEntity = (m_ViewportData.HoveredEntityID > m_Scene->GetMaxEntityId()) ? nullptr : m_Scene->GetEnitityFromId(m_ViewportData.HoveredEntityID);
    }
}
