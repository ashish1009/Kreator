//
//  RendererLayer.cpp
//  Kreator
//
//  Created by iKan on 19/05/22.
//

#include "ChessLayer.hpp"

using namespace Chess;

namespace ChessUtils {
    
    std::string ColorString(Color color) {
        switch (color) {
            case Color::Black: return "Black";
            case Color::White: return "White";
            case Color::MAX_PLAYER:
            default:
                IK_ASSERT(false, "Invalid color");
        }
    }
    
}

uint32_t Player::NumPlayerCreated = 0;

/// Chess Renderer Layer Constructor
ChessLayer::ChessLayer(const std::string& playerName_1, const std::string& playerName_2)
: Layer("Chess Renderer Layer"), m_Players{ playerName_1, playerName_2 } {
    IK_INFO("Creating '{0}' ...", m_Name);
}

/// Chess Renderer Layer Destructor
ChessLayer::~ChessLayer() {
    IK_WARN("Destroying '{0}' !!!", m_Name);
}

/// Attach the Renderer Layer to the application
void ChessLayer::Attach() {
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
    
    // Create the Scene for rendering
    m_Scene = Scene::Create();
    m_Scene->PlayScene();

    // Setup the scene Camera Entity
    m_CameraEntity = m_Scene->CreateEntity("Camera");
    m_CameraEntity->AddComponent<CameraComponent>(SceneCamera::ProjectionType::Orthographic);

    // Shifiting the camera and Border block as {0, 0} is our first block (bottom left) which is at the center
    auto& camPosition = m_CameraEntity->GetComponent<TransformComponent>().Translation;
    camPosition = { 3.5f, 3.5f, 0.0f };

    InitBlocksData();
}

/// Update the renderer Layer each frame
void ChessLayer::Update(Timestep ts) {
    // If resize the window call the update the Scene View port and Frame buffer should be resized
    if (const FrameBuffer::Specification& spec = m_ViewportData.FrameBuffer->GetSpecification();
        (uint32_t)m_ViewportData.Size.x > 0 && (uint32_t)m_ViewportData.Size.y > 0 && // zero sized framebuffer is invalid
        (spec.Width != (uint32_t)m_ViewportData.Size.x || spec.Height != (uint32_t)m_ViewportData.Size.y)) {
        m_ViewportData.FrameBuffer->Resize((uint32_t)m_ViewportData.Size.x, (uint32_t)m_ViewportData.Size.y);
        m_Scene->SetViewport((uint32_t)m_ViewportData.Size.x, (uint32_t)m_ViewportData.Size.y);    }
    
    Renderer::ResetStatsEachFrame();

    m_ViewportData.FrameBuffer->Bind();

    Renderer::Clear({ 0.1f, 0.1f, 0.1f, 1.0f });
    m_Scene->Update(ts);
    m_ViewportData.UpdateMousePos();
    UpdateHoveredEntity();

    m_ViewportData.FrameBuffer->Unbind();
}

/// Render ; for Renderer Layer
void ChessLayer::RenderGui() {
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
        
        for (uint8_t playerIdx = 0; playerIdx < MAX_PLAYER; playerIdx++) {
            const auto& player = m_Players[playerIdx];
            std::string playerIdxString = "Color : " + ChessUtils::ColorString(player.Color);
            ImGui::Text("%s", playerIdxString.c_str());
            ImGui::Text("Name  : %s", player.Name.c_str());
            ImGui::Separator();
        }
        
        if (m_ViewportData.HoveredEntity) {
            const auto& entity = m_ViewportData.HoveredEntity;
            ImGui::Text("Hovered Block : %s", entity->GetComponent<TagComponent>().Tag.c_str());
            ImGui::Separator();
                
            // get the position of Block as Row and Column
            const auto& entityPosition = entity->GetComponent<TransformComponent>().Translation;
            int8_t rowIdx = (int8_t)entityPosition.y;
            int8_t colIdx = (int8_t)entityPosition.x;

            // Extract Block from hovered Block Row and column
            const auto& block = m_Blocks[rowIdx][colIdx];
            ImGui::Text("Row : %d", block->Row);
            ImGui::Text("Col : %d", block->Col);
            ImGui::Separator();
            if (!block->Piece)
                ImGui::Text("Piece : Empty");
            ImGui::Separator();
        }
        
        ImGui::End(); // Debug Window
    }
        
    ImguiAPI::EndDcocking();
}

/// Detach the Renderer Lyer
void ChessLayer::Detach() {
    IK_WARN("Detaching '{0}'", m_Name);
}

/// Handle the Events of Renderer Layer
void ChessLayer::EventHandler(Event& event) {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<KeyPressedEvent>(IK_BIND_EVENT_FN(ChessLayer::OnKeyPressed));
    dispatcher.Dispatch<MouseButtonPressedEvent>(IK_BIND_EVENT_FN(ChessLayer::OnMouseButtonPressed));
    dispatcher.Dispatch<WindowResizeEvent>(IK_BIND_EVENT_FN(ChessLayer::OnWindowResize));

    m_Scene->EventHandler(event);
}

/// Mouse button Event
/// @param e Mouse Button event handler
bool ChessLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e) {
    return false;
}

/// Kwy Press Event
/// @param event Key Press event handler
bool ChessLayer::OnKeyPressed(KeyPressedEvent& event) {
    return false;
}

/// Mouse button Event
/// @param e Mouse Button event handler
bool ChessLayer::OnWindowResize(WindowResizeEvent& e) {
    Renderer::SetViewportSize(e.GetWidth(), e.GetHeight());
    return false;
}

/// Update Hovered Entity
void ChessLayer::UpdateHoveredEntity() {
    if (m_ViewportData.Hovered) {
        Renderer::GetEntityIdFromPixels(m_ViewportData.MousePosX, m_ViewportData.MousePosY, m_ViewportData.HoveredEntityID);
        m_ViewportData.HoveredEntity = (m_ViewportData.HoveredEntityID > m_Scene->GetMaxEntityId()) ? nullptr : m_Scene->GetEnitityFromId(m_ViewportData.HoveredEntityID);
    }
}

void ChessLayer::InitBlocksData() {
    // Render the Outline board of Chess
    std::shared_ptr<Entity> backgroundEntity = m_Scene->CreateEntity("Background");
    backgroundEntity->AddComponent<QuadComponent>(glm::vec4(0.5f, 0.6f, 0.8f, 1.0f));
    auto& tc = backgroundEntity->GetComponent<TransformComponent>();
    tc.Translation = { 3.5f, 3.5f, 0.0f };
    tc.Scale = { 8.5f, 8.5f, 1.0f };

    // Initialize the Chess Board block
    for (uint8_t rowIdx = 0; rowIdx < MAX_ROWS; rowIdx++) {
        for (uint8_t colIdx = 0; colIdx < MAX_COLUMNS; colIdx++) {
            m_Blocks[rowIdx][colIdx] = Block::Create(rowIdx, colIdx);
            
            uint8_t blockIndex = rowIdx * MAX_ROWS + colIdx;
            
            // Create Entity for each Block
            std::string blockName = "Block " + std::to_string(blockIndex);
            std::shared_ptr<Entity> blockEntity = m_Scene->CreateEntity(blockName);

            // Assign color to each block alternative
            static const glm::vec4 whiteColor = { 0.5f, 0.5f, 0.5f, 1.0f };
            static const glm::vec4 darkColor = { 0.7f, 0.1f, 0.1f, 1.0f };
            glm::vec4 blockColor = glm::vec4(0.0f);
            if (rowIdx % 2 == 0)
                blockColor = (blockIndex % 2 == 0) ? whiteColor : darkColor;
            else
                blockColor = (blockIndex % 2 == 0) ? darkColor : whiteColor;

            blockEntity->AddComponent<QuadComponent>(blockColor);
            
            // Update the block position as Entity Position
            auto& blockPosition = blockEntity->GetComponent<TransformComponent>().Translation;
            blockPosition.x = m_Blocks[rowIdx][colIdx]->Col; // X pixel is equivalent to Column
            blockPosition.y = m_Blocks[rowIdx][colIdx]->Row; // Y pixel is equivalent to Row
            
            // Store the blcok in the Entity Map;
            m_BlockEntityMap[blockEntity] = m_Blocks[rowIdx][colIdx];
        }
    }
}
