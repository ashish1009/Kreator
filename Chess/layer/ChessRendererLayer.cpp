//
//  RendererLayer.cpp
//  Kreator
//
//  Created by iKan on 19/05/22.
//

#include "ChessRendererLayer.hpp"

using namespace Chess;

namespace ChessUtils {
    
    std::string ColorNameString(Piece::Color color) {
        switch (color) {
            case Piece::Color::Black:  return "Black";
            case Piece::Color::White:  return "White";
            case Piece::Color::Max:
            default:
                IK_ASSERT(false, "Invalid Color");
        }
    }

    std::string PieceNameString(Piece::Name name) {
        switch (name) {
            case Piece::Name::Pawn: return "Pawn";
            case Piece::Name::King: return "King";
            case Piece::Name::Queen: return "Queen";
            case Piece::Name::Knight: return "Knight";
            case Piece::Name::Bishop: return "Bishop";
            case Piece::Name::Rook: return "Rook";
            case Piece::Name::Invalid:
            default:
                IK_ASSERT(false, "Invalid Color");
        }
    }
    
    /// create texture for specific peice
    /// @param color name of color (should be same as folder name where piece stored)
    /// @param name name of piece (should be same as file name of piece without .png)
    std::shared_ptr<Texture> GetTexture(const std::string& color, const std::string& name) {
        return Texture::Create(AssetManager::GetClientAsset("/texture/" + color + "/" + name + ".png"));
    }
    
}

/// Map the folder name with the color of piece (black should be at 0th position and white at 1)
static const std::string FolderName[MAX_PLAYERS] = { "black", "white" };

/// Map the file name with the column index of piece (should be same as PowerPiecePosition)
static const std::string FileName[MAX_COLUMNS] = { "rook", "knight", "bishop", "queen", "king", "bishop", "knight", "rook" };

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
    
    // Setup the scene Camera Entity
    m_CameraEntity = m_Scene->CreateEntity("Camera");
    m_CameraEntity->AddComponent<CameraComponent>(SceneCamera::ProjectionType::Orthographic);

    // Shifiting the camera and Border block as {0, 0} is our first block (bottom left) which is at the center
    auto& camPosition = m_CameraEntity->GetComponent<TransformComponent>().Translation;
    camPosition = { 3.5f, 3.5f, 0.0f };

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
            ImGui::Separator();

            if (m_BlockEntityMap.find(entity) != m_BlockEntityMap.end()) {
                const Block& block = m_BlockEntityMap[entity];
                ImGui::Text("Block Index    : %d", block.m_Index);
                ImGui::Text("Row            : %d", block.m_Row);
                ImGui::Text("Col            : %d", block.m_Col);
                
                ImGui::Separator();
            }
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
    // Render the Outline board of Chess
    std::shared_ptr<Entity> backgroundEntity = m_Scene->CreateEntity("Background");
    backgroundEntity->AddComponent<QuadComponent>(glm::vec4(0.5f, 0.6f, 0.8f, 1.0f));
    auto& tc = backgroundEntity->GetComponent<TransformComponent>();
    tc.Translation = { 3.5f, 3.5f, 0.0f };
    tc.Scale = { 8.5f, 8.5f, 1.0f };

    for (uint8_t rowIdx = 0; rowIdx < MAX_ROWS; rowIdx++) {
        for (uint8_t colIdx = 0; colIdx < MAX_COLUMNS; colIdx++) {
            // Create Block for each Entity
            Block block(rowIdx, colIdx);
            
            // Stores the Index of each block
            block.m_Index = rowIdx * MAX_ROWS + colIdx;

            // Create Entity for each Block
            std::string blockName = "Block " + std::to_string(block.m_Index);
            std::shared_ptr<Entity> entity = m_Scene->CreateEntity(blockName);
                        
            // Assign color to each block alternative
            static const glm::vec4 whiteColor = { 0.5f, 0.5f, 0.5f, 1.0f };
            static const glm::vec4 darkColor = { 0.7f, 0.1f, 0.1f, 1.0f };
            glm::vec4 blockColor = glm::vec4(0.0f);
            if (rowIdx % 2 == 0)
                blockColor = (block.m_Index % 2 == 0) ? whiteColor : darkColor;
            else
                blockColor = (block.m_Index % 2 == 0) ? darkColor : whiteColor;

            entity->AddComponent<QuadComponent>(blockColor);

            // Update the block position as Entity Position
            auto& blockPosition = entity->GetComponent<TransformComponent>().Translation;
            blockPosition.x = block.m_Col;
            blockPosition.y = block.m_Row;
            
            m_BlockEntityMap[entity] = block;
        }
    }
}

/// Initialize the Piece data
void ChessRendererLayer::InitPlayerData() {
    for (uint8_t playerIdx = 0; playerIdx < MAX_PLAYERS; playerIdx++) {
        auto& player = m_PlayerData[(Piece::Color)playerIdx];
        player.m_Color = (Piece::Color)playerIdx;

        auto pawnRowPosition = PAWN_INIT_ROW_POSITION[playerIdx];
        for (uint8_t pawnIdx = 0; pawnIdx < MAX_PAWNS; pawnIdx++) {
            std::shared_ptr<Piece> piece = Piece::Create(Piece::Name::Pawn, player.m_Color, pawnRowPosition, pawnIdx);

            auto entityName = ChessUtils::ColorNameString((Piece::Color)playerIdx) + ChessUtils::PieceNameString(Piece::Name::Pawn) + std::to_string(pawnIdx);
            std::shared_ptr<Texture> texture = ChessUtils::GetTexture(FolderName[playerIdx], "pawn");

            player.m_PieceEntityMap[CreatePieceEntity(entityName, texture, { pawnIdx, pawnRowPosition, 0.0f })] = piece;
        }

        auto otherPiecePosition = OTHER_PIECE_INIT_ROW_POSITION[playerIdx];
        for (uint8_t colIdx = 0; colIdx < MAX_COLUMNS; colIdx++) {
            auto pieceName = PowerPiecePosition[colIdx];
            
            std::shared_ptr<Piece> piece = Piece::Create(pieceName, player.m_Color, otherPiecePosition, colIdx);
            auto entityName = ChessUtils::ColorNameString((Piece::Color)playerIdx) + ChessUtils::PieceNameString(pieceName);
            
            std::shared_ptr<Texture> texture = ChessUtils::GetTexture(FolderName[playerIdx], FileName[colIdx]);
            player.m_PieceEntityMap[CreatePieceEntity(entityName, texture, { colIdx, otherPiecePosition, 0.0f })] = piece;
        }
   }
}

std::shared_ptr<Entity> ChessRendererLayer::CreatePieceEntity(const std::string& entityName, std::shared_ptr<Texture> texture, const glm::vec3& position) {
    std::shared_ptr<Entity> entity = m_Scene->CreateEntity(entityName);
    entity->AddComponent<QuadComponent>(texture);

    // Update the position of Pawn
    auto& tc = entity->GetComponent<TransformComponent>();
    tc.Translation = position;
    tc.Scale = glm::vec3(0.5f, 0.5f, 1.0f);
    
    return entity;
}
