//
//  RendererLayer.hpp
//  Kreator
//
//  Created by iKan on 19/05/22.
//

#pragma once

#include "chess/Player.hpp"
#include "chess/Block.hpp"

using namespace iKan;

namespace Chess {
    
    /// Chess Renderer Layer
    class ChessLayer : public Layer {
    public:
        ChessLayer(const std::string& playerName_1, const std::string& playerName_2);
        virtual ~ChessLayer();
        
        void Attach() override;
        void Update(Timestep ts) override;
        void RenderGui() override;
        void Detach() override;
        void EventHandler(Event&) override;
        
        const std::string& GetName() const override { return m_Name; }
        
    private:
        // Layer API
        bool OnKeyPressed(KeyPressedEvent& e);
        bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);
        void UpdateHoveredEntity();
        
        // Initialize the Chess Data
        void InitBlocksData();
        void InitPlayerData();
        std::shared_ptr<Entity> CreatePieceEntity(const std::string& entityName, std::shared_ptr<Texture> texture, const glm::vec3& position);

        // Update Chess Content
        void UpdateSelectedPiece(std::shared_ptr<Block> block);
        void DeSelectPiece();
        void EmptyBlock(std::shared_ptr<Block> block);
        void FillBlock(std::shared_ptr<Block> block, std::shared_ptr<Piece> piece);
        void ValidateAndUpdateMove(bool isBlockEmpty);
        void ShowPossibleMoves();
        void DeletePossibleMovesEntities();
        bool IsBlockEmpty(std::shared_ptr<Block> block);
        
        // Member variabls
        // Layer Data
        Viewport m_ViewportData;
        std::shared_ptr<Scene> m_Scene;
        
        // Chess Data
        std::shared_ptr<Entity> m_CameraEntity;
        std::shared_ptr<Block> m_Blocks[MAX_ROWS][MAX_COLUMNS];
        std::shared_ptr<Entity> m_EntityForOutlineHoveredBlock, m_EntityForOutlineSelectedBlock;
        std::shared_ptr<Texture> m_HoveredOutlineTexture, m_SelectedOutlineTexture, m_PossibleMoveOutlineTexture;
        
        std::vector<std::shared_ptr<Entity>> m_PossibleOutlineEntities;

        // Store the pointer of Hovered Block
        std::shared_ptr<Block> m_HoveredBlock;
        std::shared_ptr<Piece> m_SelectedPiece;
        
        Player m_Players[MAX_PLAYER];
        
        Color m_Turn = Color::Black;
    };

}
