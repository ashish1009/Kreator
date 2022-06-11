//
//  RendererLayer.cpp
//  Kreator
//
//  Created by iKan on 19/05/22.
//

#include "RendererLayer.hpp"
#include <ft2build.h>
#include <map>
#include <glad/glad.h>
#include FT_FREETYPE_H

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};
std::map<GLchar, Character> Characters;
unsigned int VAO, VBO;

glm::vec3 CameraTranslation = { 0.0f, 0.0f, 0.1f };
glm::vec3 QuadTranslation = { 0.0f, 0.0f, 0.1f };

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
    
    // OpenGL state
    // ------------
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // compile and setup the shader
    // ----------------------------
    m_TextShader = Renderer::GetShader(AssetManager::GetCoreAsset("shaders/2D/TextShader.glsl"));
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(1600), 0.0f, static_cast<float>(900));
    m_TextShader->Bind();
    glUniformMatrix4fv(glGetUniformLocation(m_TextShader->GetRendererID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    
    // FreeType
    // --------
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
    {
//        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return;
    }
    
    // find path to font
    std::string font_name = AssetManager::GetCoreAsset("fonts/opensans/OpenSans-Bold.ttf");
    if (font_name.empty())
    {
//        std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
        return;
    }
    
    // load font as face
    FT_Face face;
    if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
//        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return;
    }
    else {
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++)
        {
            // Load character glyph
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
//                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            Characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    
    // configure VAO/VBO for texture quads
    // -----------------------------------
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

/// Renderer Layer Detach
void RendererLayer::Detach() {
    PROFILE();
    IK_WARN("Detaching {0} Layer to Application", m_Name);
}


// render line of text
// -------------------
void RendererLayer::RenderText(std::string text, float x, float y, float scale, glm::vec3 color)
{
    // activate corresponding render state
    m_TextShader->Bind();
    glUniform3f(glGetUniformLocation(m_TextShader->GetRendererID(), "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
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
        
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        RenderText("This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
        RenderText("(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));

        
//        m_ActiveScene->Update(ts);
        m_VpData.UpdateMousePos();

        UpdateHoveredEntity();
    }
    m_VpData.FrameBuffer->Unbind();
}

/// Render GUI Window each frame for Renderer Layer
void RendererLayer::RenderGui() {
    ImguiAPI::StartDcocking();
    ImguiAPI::FrameRate();

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
            
            ImguiAPI::RendererVersion();
            
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
