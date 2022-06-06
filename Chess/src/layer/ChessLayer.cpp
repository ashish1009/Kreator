//
//  RendererLayer.cpp
//  Kreator
//
//  Created by iKan on 19/05/22.
//

#include "ChessLayer.hpp"
#include "chess/Pieces.hpp"

using namespace Chess;

#define BLOCK_EMPTY true
#define BLOCK_NOT_EMPTY false

namespace ChessUtils {
    
    /// return the color name as string
    /// @param color color name as enum
    std::string ColorString(Color color) {
        switch (color) {
            case Color::Black: return "Black";
            case Color::White: return "White";
            case Color::MAX_PLAYER:
            default:
                IK_ASSERT(false, "Invalid color");
        }
    }
    
    /// return the Piece Name as string
    /// @param name piece name as enum
    std::string PieceString(enum Piece::Name name) {
        switch (name) {
            case Piece::Name::Pawn: return "Pawn";
            case Piece::Name::King: return "King";
            case Piece::Name::Queen: return "Queen";
            case Piece::Name::Knight: return "Knight";
            case Piece::Name::Bishop: return "Bishop";
            case Piece::Name::Rook: return "Rook";
            default:
                IK_ASSERT(false, "Invalid Color");
        }
    }
    
    /// create texture for specific peice
    /// @param color name of color (should be same as folder name where piece stored)
    /// @param name name of piece (should be same as file name of piece without .png)
    std::shared_ptr<Texture> GetTexture(const std::string& color, const std::string& name) {
        return Renderer::GetTexture(AssetManager::GetClientAsset("/texture/" + color + "/" + name + ".png"));
    }

}

/// Map the folder name with the color of piece (black should be at 0th position and white at 1)
static const std::string FolderName[MAX_PLAYER] = { "black", "white" };

/// Map the file name with the column index of piece (should be same as PowerPiecePosition)
static const std::string FileName[MAX_COLUMNS] = { "rook", "knight", "bishop", "queen", "king", "bishop", "knight", "rook" };

// Static Member of Plaery class
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
    InitPlayerData();
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
            std::string playerIdxString = ChessUtils::ColorString(player.Color) + " : " + player.Name;
            ImGui::Text("%s", playerIdxString.c_str());
            ImGui::Separator();
        }
        
        ImGui::Text("TURN  : %s", ChessUtils::ColorString(m_Turn).c_str());
        ImGui::Separator();

        if (m_ViewportData.HoveredEntity) {
            const auto& entity = m_ViewportData.HoveredEntity;
            const auto& entityName = entity->GetComponent<TagComponent>().Tag;

            // If entity curesor reached outside the board
            // TODO: Fix this later as it will still work outside th border
            if (entityName != "Border") { // Should be same as Border entity name
                // get the position of Block as Row and Column
                const auto& entityPosition = entity->GetComponent<TransformComponent>().Translation;
                const int8_t rowIdx = (int8_t)entityPosition.y;
                const int8_t colIdx = (int8_t)entityPosition.x;
                
                // Extract Block from hovered Block Row and column
                m_HoveredBlock = m_Blocks[rowIdx][colIdx];

                // Update the position of Entity that render outline over block
                auto& HoveredEntityPos = m_EntityForOutlineHoveredBlock->GetComponent<TransformComponent>().Translation;
                HoveredEntityPos = { colIdx, rowIdx, 0.1f };

                ImGui::Text("Hovered Block");
                ImGui::Text("Row : %d", m_HoveredBlock->Row);
                ImGui::Text("Col : %d", m_HoveredBlock->Col);
                ImGui::Separator();
                if (const std::shared_ptr<Piece>& piece = m_HoveredBlock->Piece) {
                    ImGui::Text("Piece");
                    ImGui::Text("Name    : %s", ChessUtils::PieceString(piece->Name).c_str());
                    ImGui::Text("Color   : %s", ChessUtils::ColorString(piece->Color).c_str());
                    ImGui::Text("Row     : %d", piece->Row);
                    ImGui::Text("Col     : %d", piece->Col);
                    ImGui::Separator();
                }
                else
                    ImGui::Text("Piece : Empty");
            }
        }
        else {
            // deselecting Hovered Block
            m_HoveredBlock = nullptr;
        }
        
        ImGui::End(); // Debug Window
        
        ImGui::Begin("Selected Piece");
        if (m_SelectedPiece) {
            ImGui::Text("Selected Piece");
            ImGui::Separator();
            ImGui::Text("Name    : %s", ChessUtils::PieceString(m_SelectedPiece->Name).c_str());
            ImGui::Text("Color   : %s", ChessUtils::ColorString(m_SelectedPiece->Color).c_str());
            ImGui::Text("Row     : %d", m_SelectedPiece->Row);
            ImGui::Text("Col     : %d", m_SelectedPiece->Col);
        }
        ImGui::End(); // Selected Piece
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
}

/// Mouse button Event
/// @param e Mouse Button event handler
bool ChessLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e) {
    // Take Action only if Hovered block is not null then return
    if (!m_HoveredBlock)
        return false;
    
    // If block is Empty
    if (IsBlockEmpty(m_HoveredBlock)) {
        // Take Action only of Selected Piece is not null then return
        if (!m_SelectedPiece)
            return false;
        
        /// Validate the new Move and if Valid then Update the Hovered and Selected Block
        ValidateAndUpdateMove(BLOCK_EMPTY);
    }
    // If Hovered Block is not Empty
    else {
        // If Piece is selected then  Validate the new Move and
        // if Valid then Update the Hovered and Selected Block
        if (m_SelectedPiece) {
            // If Selecting Same Piece then deselect the current Piece and return
            if (m_SelectedPiece == m_HoveredBlock->Piece) {
                DeSelectPiece();
                return false;
            }
            // If selected Piece is of same color but other then current Selected Piece
            // Then update the selected Piece
            else if (m_SelectedPiece->Color == m_HoveredBlock->Piece->Color) {
                UpdateSelectedPiece(m_HoveredBlock);
            }
            // Validate if Hovered Piece of different color
            else {
                ValidateAndUpdateMove(BLOCK_NOT_EMPTY);
            }
        }
        // If no Piece is selected the Update selected piece and return
        else {
            // Check is it turn of right player
            if (m_HoveredBlock->Piece->Color != m_Turn)
                return false;
            
            UpdateSelectedPiece(m_HoveredBlock);
        }
    }
    return false;
}

/// Kwy Press Event
/// @param event Key Press event handler
bool ChessLayer::OnKeyPressed(KeyPressedEvent& event) {
    if (event.GetKeyCode() == KeyCode::Escape) {
        DeSelectPiece();
    }
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

/// Initialize the Block data
void ChessLayer::InitBlocksData() {
    // Render the Outline board of Chess
    std::shared_ptr<Entity> backgroundEntity = m_Scene->CreateEntity("Border");
    backgroundEntity->AddComponent<QuadComponent>(glm::vec4(0.5f, 0.6f, 0.8f, 1.0f));
    auto& tc = backgroundEntity->GetComponent<TransformComponent>();
    tc.Translation = { 3.5f, 3.5f, 0.0f };
    tc.Scale = { 8.5f, 8.5f, 1.0f };
    
    // Create Entity to render outline over hovered Block
    m_EntityForOutlineHoveredBlock = m_Scene->CreateEntity("EntityForOutlineHoveredBlock");
    m_HoveredOutlineTexture = Renderer::GetTexture(AssetManager::GetClientAsset("texture/common/hoveredBlock.png"));
    m_EntityForOutlineHoveredBlock->AddComponent<QuadComponent>(m_HoveredOutlineTexture);

    // Create Entity to render outline over Selected Block
    m_EntityForOutlineSelectedBlock = m_Scene->CreateEntity("EntityForOutlineSelectedBlock");
    m_SelectedOutlineTexture = Renderer::GetTexture(AssetManager::GetClientAsset("texture/common/selectedBlock.png"));
    
    // Possible Move outline Texture
    m_PossibleMoveOutlineTexture = Renderer::GetTexture(AssetManager::GetClientAsset("texture/common/possibleMove.png"));

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
            static const glm::vec4 darkColor = { 0.1f, 0.3f, 0.5f, 1.0f };
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
        }
    }
}

/// Initialize the Player data
void ChessLayer::InitPlayerData() {
    for (uint8_t playerIdx = 0; playerIdx < MAX_PLAYER; playerIdx++) {
        auto& player = m_Players[playerIdx];
        auto pawnRowPosition = PAWN_INIT_ROW_POSITION[playerIdx];
        
        // Create the Pawn for each Player
        for (uint8_t pawnIdx = 0; pawnIdx < MAX_PAWNS; pawnIdx++) {
            std::shared_ptr<Piece> piece = Piece::Create(Piece::Name::Pawn, player.Color, pawnRowPosition, pawnIdx);
            
            auto entityName = ChessUtils::ColorString((Chess::Color)playerIdx) + ChessUtils::PieceString(Piece::Name::Pawn) + std::to_string(pawnIdx);
            std::shared_ptr<Texture> texture = ChessUtils::GetTexture(FolderName[playerIdx], "pawn");
            
            piece->Entity = CreatePieceEntity(entityName, texture, { pawnIdx, pawnRowPosition, 0.1f });
            
            // Store the current piece in Block
            m_Blocks[pawnRowPosition][pawnIdx]->Piece = piece;
        }

        // Create all the power piece for each player
        auto otherPiecePosition = POWER_PIECE_INIT_ROW_POSITION[playerIdx];
        for (uint8_t colIdx = 0; colIdx < MAX_COLUMNS; colIdx++) {
            auto pieceName = PowerPiecePosition[colIdx];
            
            std::shared_ptr<Piece> piece = Piece::Create(pieceName, player.Color, otherPiecePosition, colIdx);
            auto entityName = ChessUtils::ColorString((Chess::Color)playerIdx) + ChessUtils::PieceString(pieceName);
            
            std::shared_ptr<Texture> texture = ChessUtils::GetTexture(FolderName[playerIdx], FileName[colIdx]);
            piece->Entity = CreatePieceEntity(entityName, texture, { colIdx, otherPiecePosition, 0.0f });

            // Store the current piece in Block
            m_Blocks[otherPiecePosition][colIdx]->Piece = piece;
        }
    }
}

/// Update the Selected Piece
void ChessLayer::UpdateSelectedPiece(std::shared_ptr<Block> block) {
    // delete possible move block entity before Selecting Block
    DeletePossibleMovesEntities();
    
    m_SelectedPiece = block->Piece;
    
    if (!m_EntityForOutlineSelectedBlock->HasComponent<QuadComponent>())
        m_EntityForOutlineSelectedBlock->AddComponent<QuadComponent>(m_SelectedOutlineTexture);

    auto& position = m_EntityForOutlineSelectedBlock->GetComponent<TransformComponent>().Translation;
    position = { block->Col, block->Row, 0.1f };
    
    ShowPossibleMoves();
}

/// Deselect the Selectd Piece
void ChessLayer::DeSelectPiece() {
    // delete possible move block entity before deselecing block
    DeletePossibleMovesEntities();
    
    // Deselecting the selected Piece
    m_SelectedPiece = nullptr;
    
    if (m_EntityForOutlineSelectedBlock->HasComponent<QuadComponent>())
        m_EntityForOutlineSelectedBlock->RemoveComponent<QuadComponent>();
}

/// Make the block empty by removing the piece
/// @param block current Block
void ChessLayer::EmptyBlock(std::shared_ptr<Block> block) {
    IK_INFO("Emptying {0} {1}", block->Row, block->Col);
    block->Piece = nullptr;
}

/// update the piece value of Block
/// @param block current Block
/// @param piece new piece
void ChessLayer::FillBlock(std::shared_ptr<Block> block, std::shared_ptr<Piece> piece) {
    block->Piece = piece;
}

/// Validate the new Move and if Valid then Update the Hovered and Selected Block
/// @param isBlockEmpty flag to get is block empty
void ChessLayer::ValidateAndUpdateMove(bool isBlockEmpty) {
    auto prevblockPtr = m_Blocks[m_SelectedPiece->Row][m_SelectedPiece->Col];
    // if Validation of new postion fails then return
    if (!m_SelectedPiece->ValidateAndUpdatePostion(m_HoveredBlock->Row, m_HoveredBlock->Col, m_Blocks))
        return;
    
    // Store the Piece entity before Updating the piece
    std::shared_ptr<Entity> entityToBeDeleted;
    if (!isBlockEmpty) {
        entityToBeDeleted = m_HoveredBlock->Piece->Entity;
        m_Scene->DestroyEntity(entityToBeDeleted);
    }
    
    // If validate result is true then update Selected Block and new hovered block Piece
    EmptyBlock(prevblockPtr);
    FillBlock(m_HoveredBlock, m_SelectedPiece);
    
    // Deselect the block piece
    DeSelectPiece();
    
    // Update the turn of player
    m_Turn = (m_Turn == Color::Black) ? Color::White : Color::Black;
}

/// Show and render outline for possible move of current Piece
void ChessLayer::ShowPossibleMoves() {
    std::vector possibleBlocks = m_SelectedPiece->GetPossibleMovePosition();
    // Create entity for each possible Move
    for (const auto& [row, col] : possibleBlocks) {
        const auto& block = m_Blocks[row][col];
        // Validate the move block
        if (m_SelectedPiece->Validate(row, col, m_Blocks)) {
            // TODO: Might remove continue in future
            if (block->Piece && block->Piece->Color == m_SelectedPiece->Color)
                continue;
            
            std::string name = std::to_string(row * MAX_ROWS + col);
            const std::shared_ptr<Entity>& entity = m_PossibleOutlineEntities.emplace_back(m_Scene->CreateEntity(name));
            auto& position = entity->GetComponent<TransformComponent>().Translation;
            position = { col, row, 0.1f };
            
            // Setup quad component with texture
            entity->AddComponent<QuadComponent>(m_PossibleMoveOutlineTexture);
        }
    }
}

/// Delete the possible mpve block entities
void ChessLayer::DeletePossibleMovesEntities() {
    for (auto& entity : m_PossibleOutlineEntities)
        m_Scene->DestroyEntity(entity);
    
    m_PossibleOutlineEntities.clear();
}

/// Check if block is empty or not
/// @param block block pointer
bool ChessLayer::IsBlockEmpty(std::shared_ptr<Block> block) {
    return block->Piece == nullptr;
}

/// create entity for Piece
/// @param entityName name of entity
/// @param texture texture to be loaded
/// @param position position of entity
std::shared_ptr<Entity> ChessLayer::CreatePieceEntity(const std::string& entityName, std::shared_ptr<Texture> texture, const glm::vec3& position) {
    std::shared_ptr<Entity> entity = m_Scene->CreateEntity(entityName);
    entity->AddComponent<QuadComponent>(texture);

    // Update the position of Pawn
    auto& tc = entity->GetComponent<TransformComponent>();
    tc.Translation = position;
    tc.Scale = glm::vec3(0.5f, 0.5f, 1.0f);
    
    return entity;
}
