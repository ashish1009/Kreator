//
//  RendererLayer.cpp
//  Kreator
//
//  Created by iKan on 19/05/22.
//

#include "RendererLayer.hpp"

/// Renderer Layer Constructor
RendererLayer::RendererLayer() : Layer("Renderer"), m_CBP("../../../../../../../iKan./iKan/Github/Product/iKan") {
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
    
    // Change the Imgui Font
    auto& imguiLayer = Application::Get().GetImGuiLayer();
    imguiLayer.SetFont(AssetManager::GetClientAsset("/fonts/OpenSans/OpenSans-Regular.ttf"), AssetManager::GetClientAsset("fonts/OpenSans/OpenSans-Bold.ttf"));
    
    // Setup the Theme
    ImguiAPI::SetGreyThemeColors();
    
    // Update the Viewport Data
    FrameBuffer::Specification spec;
    spec.Attachments = {
        FrameBuffer::Attachment::TextureFormat::RGBA8,
        FrameBuffer::Attachment::TextureFormat::Depth24Stencil,
        FrameBuffer::Attachment::TextureFormat::R32I
    };
    m_VpData.FrameBuffer = FrameBuffer::Create(spec);
    
    // TODO: Temp
    NewScene();
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
    
    static glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_VpData.Size.x), 0.0f, static_cast<float>(m_VpData.Size.y));

    // If resize the window call the update the Scene View port and Frame buffer should be resized
    if (const FrameBuffer::Specification& spec = m_VpData.FrameBuffer->GetSpecification();
        (uint32_t)m_VpData.Size.x > 0 && (uint32_t)m_VpData.Size.y > 0 && // zero sized framebuffer is invalid
        (spec.Width != (uint32_t)m_VpData.Size.x || spec.Height != (uint32_t)m_VpData.Size.y)) {
        
        m_VpData.FrameBuffer->Resize((uint32_t)m_VpData.Size.x, (uint32_t)m_VpData.Size.y);
        m_ActiveScene->SetViewport((uint32_t)m_VpData.Size.x, (uint32_t)m_VpData.Size.y);
        
        projection = glm::ortho(0.0f, static_cast<float>(m_VpData.Size.x), 0.0f, static_cast<float>(m_VpData.Size.y));
    }

    Renderer::ResetStatsEachFrame();
    
    m_VpData.FrameBuffer->Bind();
    {
        Renderer::Clear({ 0.2f, 0.2f, 0.2f, 1.0f });
        
        m_ActiveScene->Update(ts);
        m_VpData.UpdateMousePos();

        UpdateHoveredEntity();
        
        // Render the Frame rate
        Renderer::RenderText(std::to_string((uint32_t)(ImGui::GetIO().Framerate)), projection, glm::vec3(1.0f, 1.0f, 0.1f), glm::vec2(0.3), { 0.5f, 0.5f, 0.5f, 0.7f });

        // Render the Renderer Version
        static const Renderer::Capabilities& rendererCapability = Renderer::Capabilities::Get();
        static std::string rendererInfo = "(c) Kreator | " + rendererCapability.Vendor + " | " + rendererCapability.Renderer + " | " + rendererCapability.Version;
        Renderer::RenderText(rendererInfo, projection, glm::vec3(m_VpData.Size.x - 480.0f, 1.0f, 0.1f), glm::vec2(0.3), { 0.5f, 0.5f, 0.5f, 0.7f });
    }
    m_VpData.FrameBuffer->Unbind();
}

/// Render GUI Window each frame for Renderer Layer
void RendererLayer::RenderGui() {
    ImguiAPI::StartDcocking();

    if (m_ActiveScene) {
        m_ActiveScene->RenderImgui();
        
        // Viewport
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Kreator Viewport");
        ImGui::PushID("Kreator Viewport");
        
        m_VpData.Focused = ImGui::IsWindowFocused();
        m_VpData.Hovered = ImGui::IsWindowHovered();
        
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_VpData.Size = { viewportPanelSize.x, viewportPanelSize.y };
        
        size_t textureID = m_VpData.FrameBuffer->GetColorAttachmentIds()[0];
        PropertyGrid::Image((void*)textureID, { m_VpData.Size.x, m_VpData.Size.y }, { 0, 1 }, { 1, 0 });
        ImGui::PopStyleVar(); // ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        if (m_ActiveScene->IsEditing()) {
            PropertyGrid::DropConent([this](const std::string& path)
                                              {
                if (StringUtils::GetExtensionFromFilePath(path) == "Kreator")
                    OpenScene(path);
                else
                    IK_WARN("Invalid file for Scene {0}", path.c_str());
            });
            
            Renderer::ImguiRendererStats();
            m_VpData.RenderImgui();
            
            m_SHP->RenderImgui();
            m_CBP.RenderImgui();
                        
            SaveScene();
            OnImguizmoUpdate();
        }
        
        m_VpData.UpdateBound();
        
        ImGui::PopID();
        ImGui::End(); // ImGui::Begin("Kreator Viewport");
    }

    ImguiAPI::EndDcocking();
}

/// Handle Event interupt for Renderer Layer
/// @param event Event base class Instance.
void RendererLayer::EventHandler(Event& event) {
    if (!m_VpData.Hovered)
        return;

    if (m_ActiveScene)
        m_ActiveScene->EventHandler(event);

    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<MouseButtonPressedEvent>(IK_BIND_EVENT_FN(RendererLayer::OnMouseButtonPressed));
    dispatcher.Dispatch<KeyPressedEvent>(IK_BIND_EVENT_FN(RendererLayer::OnKeyPressed));
}

/// Mouse button Event
/// @param e Mouse Button event handler
bool RendererLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e) {
    if (!m_ActiveScene)
        return false;
    
    if (e.GetMouseButton() == MouseButton::ButtonLeft && !Input::IsKeyPressed(KeyCode::LeftAlt)) {
        if (m_VpData.MousePosX >= 0 && m_VpData.MousePosY >= 0 && m_VpData.MousePosX <= m_VpData.Width && m_VpData.MousePosY <= m_VpData.Height )
            m_SHP->SetSelectedEntity(m_VpData.HoveredEntity);
        else
            m_VpData.GizmoType = -1;
    }
    return false;
}

/// Kwy Press Event
/// @param event Key Press event handler
bool RendererLayer::OnKeyPressed(KeyPressedEvent& event) {
    // Shortcuts
    if (event.GetRepeatCount() > 0)
        return false;
    
    bool cmd = Input::IsKeyPressed(KeyCode::LeftSuper) || Input::IsKeyPressed(KeyCode::RightSuper);
    bool ctrl = Input::IsKeyPressed(KeyCode::LeftControl) || Input::IsKeyPressed(KeyCode::RightControl);

    switch (event.GetKeyCode()) {
        case KeyCode::N:    if (cmd)    NewScene();             break;
        case KeyCode::X:    if (cmd)    CloseScene();           break;
            
        // Gizmos
        case KeyCode::Q:    if (ctrl)    m_VpData.GizmoType = -1;                               break;
        case KeyCode::W:    if (ctrl)    m_VpData.GizmoType = ImGuizmo::OPERATION::TRANSLATE;   break;
        case KeyCode::E:    if (ctrl)    m_VpData.GizmoType = ImGuizmo::OPERATION::ROTATE;      break;
        case KeyCode::R:    if (ctrl)    m_VpData.GizmoType = ImGuizmo::OPERATION::SCALE;       break;
        default:
            break;
    }
    return false;
}

/// Update Hovered Entity
void RendererLayer::UpdateHoveredEntity() {
    if (!m_VpData.Hovered)
        return;
    
    if (ImGuizmo::IsOver()) {
        m_VpData.HoveredEntity = m_SHP->GetSelectedEntity();
        return;
    }
    
    Renderer::GetEntityIdFromPixels(m_VpData.MousePosX, m_VpData.MousePosY, m_VpData.HoveredEntityID);
    m_VpData.HoveredEntity = (m_VpData.HoveredEntityID > m_ActiveScene->GetMaxEntityId()) ? nullptr : m_ActiveScene->GetEnitityFromId(m_VpData.HoveredEntityID);
}

/// Update Imguizmo renderer
void RendererLayer::OnImguizmoUpdate() {
    std::shared_ptr<Entity> selectedEntity = m_SHP->GetSelectedEntity();
    if (selectedEntity && m_VpData.GizmoType != -1) {
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();
        
        float windowWidth = (float)ImGui::GetWindowWidth();
        float windowHeight = (float)ImGui::GetWindowHeight();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
        
        // Camera
        const std::shared_ptr<EditorCamera>& editorCamera = m_ActiveScene->GetEditorCamera();
        
        glm::mat4 cameraProjection = editorCamera->GetProjectionMatrix();
        glm::mat4 cameraView       = editorCamera->GetViewMatrix();
        
        // Entity transform
        auto& tc = selectedEntity->GetComponent<TransformComponent>();
        glm::mat4 transform = tc.GetTransform();
        
        // Snapping
        bool snap = Input::IsKeyPressed(KeyCode::LeftControl);
        float snapValue = 0.5f; // Snap to 0.5m for translation/scale
        
        // Snap to 45 degrees for rotation
        if (m_VpData.GizmoType == ImGuizmo::OPERATION::ROTATE)
            snapValue = 45.0f;
        
        float snapValues[3] = { snapValue, snapValue, snapValue };
        
        ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
                             (ImGuizmo::OPERATION)m_VpData.GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
                             nullptr, snap ? snapValues : nullptr);
        
        if (ImGuizmo::IsUsing()) {
            glm::vec3 translation, rotation, scale;
            Math::DecomposeTransform(transform, translation, rotation, scale);
            
            glm::vec3 deltaRotation = rotation - tc.Rotation;
            tc.Translation = translation;
            tc.Rotation += deltaRotation;
            tc.Scale = scale;
        }
    }
}

/// Create New scene
const std::shared_ptr<Scene>& RendererLayer::NewScene() {
    CloseScene();
    // TODO: Save scene if already saved

    m_ActiveScene = Scene::Create();
    m_ActiveScene->SetViewport((uint32_t)m_VpData.Size.x, (uint32_t)m_VpData.Size.y);

    m_SHP = SceneHierarchyPannel::Create(m_ActiveScene);

    return m_ActiveScene;
}

/// Open Scene
/// @param path Path of saved scene
const std::shared_ptr<Scene>& RendererLayer::OpenScene(const std::string& path) {
    IK_INFO("Opening saved scene from {0}", path.c_str());

    m_SHP->SetSelectedEntity(nullptr);
    NewScene();
    m_ActiveScene->SetFilePath(path);

    SceneSerializer serializer(m_ActiveScene);
    serializer.Deserialize(path);

    return m_ActiveScene;
}

/// Save the scene
const std::shared_ptr<Scene>& RendererLayer::SaveScene() {
    ImGui::Begin("Save File");
    ImGui::PushID("Save File");

    const auto& relativePath = (std::filesystem::relative(m_CBP.GetCurrentDir(), m_CBP.GetRootDir())).string();
    PropertyGrid::String("Scene Directory", relativePath, "File will be saved at the Current directory in the active scene", 150.0f);

    static std::string fileName = "";
    bool modified = PropertyGrid::String("Scene Name", fileName, 150.0f);

    if (modified) {
        std::string filepath = m_CBP.GetCurrentDir().string() + "/" + fileName + ".Kreator";
        IK_INFO("Saving Scene at {0}", filepath.c_str());
        if (!filepath.empty()) {
            m_ActiveScene->SetFilePath(filepath);
            SceneSerializer serializer(m_ActiveScene);
            serializer.Serialize(filepath);
        }
    }

    ImGui::PopID();
    ImGui::End();
    return m_ActiveScene;
}

/// Close current open scene
void RendererLayer::CloseScene() {
    if (!m_ActiveScene)
        return;

    if (m_ActiveScene->GetFilePath() != "Unsaved Scene") {
        SceneSerializer serializer(m_ActiveScene);
        serializer.Serialize(m_ActiveScene->GetFilePath());
    }
    else {
        SceneSerializer serializer(m_ActiveScene);
        serializer.Serialize("../../Kreator/assets/scenes/unsavedScene/unsavedScene.Kreator");
    }

    m_ActiveScene.reset();
    m_ActiveScene = nullptr;

    m_SHP.reset();
    m_SHP = nullptr;
}
