//
//  RendererLayer.cpp
//  Kreator
//
//  Created by iKan on 19/05/22.
//

#include "ChessRendererLayer.hpp"

using namespace Chess;

/// Chess Renderer Layer Constructor
ChessRendererLayer::ChessRendererLayer(const std::string& playerName_1, const std::string& playerName_2)
: Layer("Chess Renderer Layer"), m_PlayerData({ playerName_1, playerName_2 }) {
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
    
    // Create scene
    m_Scene = Scene::Create();
    m_Scene->PlayScene();
    
    // Render the Outline board of Chess
    std::shared_ptr<Entity> backgroundEntity = m_Scene->CreateEntity("Background");
    backgroundEntity->AddComponent<QuadComponent>(glm::vec4(0.5f, 0.6f, 0.8f, 1.0f));
    
    // Setup the scene Camera Entity
    m_CameraEntity = m_Scene->CreateEntity("Camera");
    m_CameraEntity->AddComponent<CameraComponent>(SceneCamera::ProjectionType::Orthographic);

    // Shifiting the camera and Border block as {0, 0} is our first block (bottom left) which is at the center
    auto& camPosition = m_CameraEntity->GetComponent<TransformComponent>().Translation;
    camPosition = { 3.5f, 3.5f, 0.0f };
    auto& tc = backgroundEntity->GetComponent<TransformComponent>();
    tc.Translation = { 3.5f, 3.5f, 0.0f };
    tc.Scale = { 8.5f, 8.5f, 1.0f };

    InitBlockData();
    InitPlayerData();
    
    // Before starting Game loop Update the Scene viewport
    m_Scene->SetViewport(1200, 800);
}

/// Update the renderer Layer each frame
void ChessRendererLayer::Update(Timestep ts) {
    // If resize the window call the update the Scene View port and Frame buffer should be resized
    if (const FrameBuffer::Specification& spec = m_ViewportData.FrameBuffer->GetSpecification();
        (uint32_t)m_ViewportData.Size.x > 0 && (uint32_t)m_ViewportData.Size.y > 0 && // zero sized framebuffer is invalid
        (spec.Width != (uint32_t)m_ViewportData.Size.x || spec.Height != (uint32_t)m_ViewportData.Size.y)) {
        m_ViewportData.FrameBuffer->Resize((uint32_t)m_ViewportData.Size.x, (uint32_t)m_ViewportData.Size.y);
        m_Scene->SetViewport((uint32_t)m_ViewportData.Size.x, (uint32_t)m_ViewportData.Size.y);
    }
    
    Renderer::ResetStatsEachFrame();

    m_ViewportData.FrameBuffer->Bind();

    Renderer::Clear({ 0.1f, 0.1f, 0.1f, 1.0f });
    m_Scene->Update(ts);
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
        
        if (m_ViewportData.HoveredEntity) {
            const auto& entity = m_ViewportData.HoveredEntity;
            ImGui::Text("Hovered Entity : %s", entity->GetComponent<TagComponent>().Tag.c_str());
            
            const Block& block = m_BlockEntityMap[entity];
            ImGui::Text("Block Index    : %d", block.m_BlockIndex);
            ImGui::Text("Position       : %d | %d", block.m_X, block.m_Y);
            
            ImGui::Separator();
        }
        
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

    m_Scene->EventHandler(event);
}

/// Mouse button Event
/// @param e Mouse Button event handler
bool ChessRendererLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e) {
    if (e.GetMouseButton() == MouseButton::ButtonLeft && !Input::IsKeyPressed(KeyCode::LeftAlt)) {
        if (m_ViewportData.Hovered) {
            m_Scene->SetSelectedEntity(m_ViewportData.HoveredEntity);
        }
    }
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
        m_ViewportData.HoveredEntity = (m_ViewportData.HoveredEntityID > m_Scene->GetMaxEntityId()) ? nullptr : m_Scene->GetEnitityFromId(m_ViewportData.HoveredEntityID);
    }
}

/// Initialize the Block data
void ChessRendererLayer::InitBlockData() {
    // NOTE: Row Index is the Y position of Renderer Screen
    //       Column Index is the X Position of Renderer Screen
    for (uint8_t rowIdx = 0; rowIdx < MAX_ROWS; rowIdx++) {
        for (uint8_t colIdx = 0; colIdx < MAX_COLUMNS; colIdx++) {
            // Create Block for each Entity
            Block block;
            
            // Stores the Index of each block
            block.m_BlockIndex = rowIdx * MAX_ROWS + colIdx;
            
            // Stores the Position of Blocks
            block.m_X = colIdx;
            block.m_Y = rowIdx;
            
            // Create Entity for each Block
            std::string blockName = "Block " + std::to_string(block.m_BlockIndex);
            std::shared_ptr<Entity> entity = m_Scene->CreateEntity(blockName);
            
            // Add the Quad Component in block for rendering quad
            auto& qc = entity->AddComponent<QuadComponent>();
            
            // Assign color to each block alternative
            // First check alternate rows.
            if (rowIdx % 2 == 0) {
                // Then check each alternate columns
                if (block.m_BlockIndex % 2 == 0)
                    // If rows % 2 is 0 then first block of row is bright
                    qc.Color = { 1.0f , 1.0f, 1.0f, 1.0f };
                else
                    // else first block of row is dark
                    qc.Color = { 0.7f, 0.1f, 0.1f, 1.0f };
            }
            else {
                // Then check each alternate columns
                if (block.m_BlockIndex % 2 == 0)
                    // If rows % 2 is NOT 0 then first block of row is dark
                    qc.Color = { 0.7f, 0.1f, 0.1f, 1.0f };
                else
                    // else first block of row is bright
                    qc.Color = { 1.0f , 1.0f, 1.0f, 1.0f };
            }

            // Update the block position as Entity Position
            auto& blockPosition = entity->GetComponent<TransformComponent>().Translation;
            blockPosition.x = block.m_X;
            blockPosition.y = block.m_Y;
            
            m_BlockEntityMap[entity] = block;
        }
    }
}

/// Initialize the Piece data
void ChessRendererLayer::InitPlayerData() {
    // Setup Player Data
    {
        // Black Data
        for (uint8_t playerIdx = 0; playerIdx < MAX_PLAYERS; playerIdx++) {
            auto& player = m_PlayerData[(Piece::Color)playerIdx];
            player.m_Color = (Piece::Color)playerIdx;
            
            auto pawnPosition = PAWN_INIT_POSITION[playerIdx];
            for (uint8_t pawnIdx = 0; pawnIdx < MAX_PAWNS; pawnIdx++)
                player.m_Pawns.emplace_back(Piece::Create(Piece::Name::Pawn, player.m_Color, pawnPosition, pawnIdx));
            
            auto otherPiecePosition = OTHER_PIECE_INIT_POSITION[playerIdx];
            player.m_King = Piece::Create(Piece::Name::King, player.m_Color, otherPiecePosition, 3);
            player.m_Queen = Piece::Create(Piece::Name::King, player.m_Color, otherPiecePosition, 4);
            
            // Bishops
            player.m_Bishop.emplace_back(Piece::Create(Piece::Name::Bishop, player.m_Color, otherPiecePosition, 0));
            player.m_Bishop.emplace_back(Piece::Create(Piece::Name::Bishop, player.m_Color, otherPiecePosition, 7));
            
            // Knight
            player.m_Knight.emplace_back(Piece::Create(Piece::Name::Knight, player.m_Color, otherPiecePosition, 1));
            player.m_Knight.emplace_back(Piece::Create(Piece::Name::Knight, player.m_Color, otherPiecePosition, 6));
            
            // Rook
            player.m_Rook.emplace_back(Piece::Create(Piece::Name::Rook, player.m_Color, otherPiecePosition, 2));
            player.m_Rook.emplace_back(Piece::Create(Piece::Name::Rook, player.m_Color, otherPiecePosition, 4));
        }
    }

}
