//
//  RendererLayer.cpp
//  Kreator
//
//  Created by iKan on 19/05/22.
//

#include "MarioLayer.hpp"
#include "mario/Background.hpp"
#include "mario/TextRendering.hpp"
#include "mario/StartScreen.hpp"

using namespace Mario;

/// Chess Renderer Layer Constructor
MarioLayer::MarioLayer()
: Layer("Mario Renderer Layer") {
    IK_INFO("Creating '{0}' ...", m_Name);
}

/// Chess Renderer Layer Destructor
MarioLayer::~MarioLayer() {
    IK_WARN("Destroying '{0}' !!!", m_Name);
}

/// Attach the Renderer Layer to the application
void MarioLayer::Attach() {
    IK_INFO("Attaching '{0}'", m_Name);
    
    BatchRenderer::ReInitialize(2000, 2000);
    
    // Change the Imgui Font
    auto& imguiLayer = Application::Get().GetImGuiLayer();
    imguiLayer.SetFont(AssetManager::GetClientAsset("/fonts/Mario.ttf"), AssetManager::GetClientAsset("fonts/Mario.ttf"));
    
    // Change Freetype font
    TextRenderer::LoadFreetype(AssetManager::GetClientAsset("/fonts/Mario.ttf"));
    
    // Setup the Theme
    ImguiAPI::SetGreyThemeColors();
    
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
    
    // Create the Scene for rendering
    m_Scene = Scene::Create();
    m_Scene->PlayScene();

    // Setup the scene Camera Entity
    m_CameraEntity = m_Scene->CreateEntity("Camera");
    auto& cameraPostion = m_CameraEntity->GetComponent<TransformComponent>().Translation;
    cameraPostion.x = 18.0f; // Start position of Mario
    
    auto& cameraComponent = m_CameraEntity->AddComponent<CameraComponent>(SceneCamera::ProjectionType::Orthographic);
    cameraComponent.Camera->SetOrthographicSize(18.0f);
    
    // initialize the Mario data
    Background::Init(m_Scene, ThemeColor::Blue);
    StartScreen::Init(m_Scene);
}

/// Update the renderer Layer each frame
void MarioLayer::Update(Timestep ts) {
    static glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_ViewportData.Size.x), 0.0f, static_cast<float>(m_ViewportData.Size.y));

    // If resize the window call the update the Scene View port and Frame buffer should be resized
    if (const FrameBuffer::Specification& spec = m_ViewportData.FrameBuffer->GetSpecification();
        (uint32_t)m_ViewportData.Size.x > 0 && (uint32_t)m_ViewportData.Size.y > 0 && // zero sized framebuffer is invalid
        (spec.Width != (uint32_t)m_ViewportData.Size.x || spec.Height != (uint32_t)m_ViewportData.Size.y)) {
        m_ViewportData.FrameBuffer->Resize((uint32_t)m_ViewportData.Size.x, (uint32_t)m_ViewportData.Size.y);
        m_Scene->SetViewport((uint32_t)m_ViewportData.Size.x, (uint32_t)m_ViewportData.Size.y);
        
        projection = glm::ortho(0.0f, static_cast<float>(m_ViewportData.Size.x), 0.0f, static_cast<float>(m_ViewportData.Size.y));
    }
    
    Renderer::ResetStatsEachFrame();

    m_ViewportData.FrameBuffer->Bind();

    Renderer::Clear({ 0.2f, 0.4f, 0.7f, 1.0f });
    m_Scene->Update(ts);
    m_ViewportData.UpdateMousePos();
    UpdateHoveredEntity();

    // Render the Text
    // Hard coding will be replaced
    TextRender::UpdateRunTime(projection, m_ViewportData.Size, { { 0, 0 }, { 1, 1 }, 300 });

    if (m_Data.IsStarted) {
    }
    else {
        TextRender::UpdateInitTime();
    }

    // Render the Frame rate
    Renderer::RenderText(std::to_string((uint32_t)(ImGui::GetIO().Framerate)), projection, glm::vec3(1.0f, 1.0f, 0.3f), glm::vec2(0.3), { 0.0f, 1.0f, 1.0f, 1.0f });
    
    m_ViewportData.FrameBuffer->Unbind();
}

/// Render ; for Renderer Layer
void MarioLayer::RenderGui() {
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
 
    ImguiAPI::EndDcocking();
}

/// Detach the Renderer Lyer
void MarioLayer::Detach() {
    IK_WARN("Detaching '{0}'", m_Name);
    Background::Shutdown(m_Scene);
    StartScreen::Shutdown(m_Scene);
}

/// Handle the Events of Renderer Layer
void MarioLayer::EventHandler(Event& event) {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<KeyPressedEvent>(IK_BIND_EVENT_FN(MarioLayer::OnKeyPressed));
    dispatcher.Dispatch<WindowResizeEvent>(IK_BIND_EVENT_FN(MarioLayer::OnWindowResize));
}

/// Kwy Press Event
/// @param event Key Press event handler
bool MarioLayer::OnKeyPressed(KeyPressedEvent& event) {
    return false;
}

/// Mouse button Event
/// @param e Mouse Button event handler
bool MarioLayer::OnWindowResize(WindowResizeEvent& e) {
    Renderer::SetViewportSize(e.GetWidth(), e.GetHeight());
    return false;
}

/// Update Hovered Entity
void MarioLayer::UpdateHoveredEntity() {
    if (m_ViewportData.Hovered) {
        Renderer::GetEntityIdFromPixels(m_ViewportData.MousePosX, m_ViewportData.MousePosY, m_ViewportData.HoveredEntityID);
        m_ViewportData.HoveredEntity = (m_ViewportData.HoveredEntityID > m_Scene->GetMaxEntityId()) ? nullptr : m_Scene->GetEnitityFromId(m_ViewportData.HoveredEntityID);
    }
}
