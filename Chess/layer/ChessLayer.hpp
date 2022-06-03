//
//  RendererLayer.hpp
//  Kreator
//
//  Created by iKan on 19/05/22.
//

#pragma once

#include <iKanHeaders.h>

using namespace iKan;

namespace Chess {
    
    /// Color of player and Pieces
    /// NOTE: each color is for a player unique so last element is MAX Player
    /// as number of color can never exceed Number of player
    enum Color { Black = 0, White = 1, MAX_PLAYER = 2 };
    
    // Constants
    static constexpr uint8_t MAX_ROWS = 8;
    static constexpr uint8_t MAX_COLUMNS = 8;
    
    // Max number of pawns at the init time fo each player
    static constexpr uint8_t MAX_PAWNS = 8;
    
    // Row Index of Pawns at start of game for each player
    static constexpr uint8_t PAWN_INIT_ROW_POSITION[MAX_PLAYER] = { 1, 6 };
    // Row Index of Power piece at start of game for each player
    static constexpr uint8_t POWER_PIECE_INIT_ROW_POSITION[MAX_PLAYER] = { 0, 7 };
    
    /// Player information wrapper
    struct Player {
    public:
        /// Stores the number of player crerted in the game
        static uint32_t NumPlayerCreated;
    
        // Both Name and Color to be initialized in constructor only;
        std::string Name = "Player";
        Color Color;
        
        /// Construct the Player
        /// @param name Name of player
        Player(const std::string& name) : Name(name) {
            IK_ASSERT(NumPlayerCreated < MAX_PLAYER, "Num Player Reached max");
            /// Assign the color from enum one by one based on index of player
            Color = (Chess::Color)NumPlayerCreated++;
        }
        
    };
    
    /// Store information about each piece of Chess
    struct Piece {
    public:
        /// Name of Piece
        enum Name { Pawn = 0, King = 1, Queen = 2, Bishop = 3, Knight = 4, Rook = 5 };
        
        int8_t Row = -1, Col = -1;

        // Name and Color to be initialised at Constructor
        Name Name;
        Color Color;
        
        /// Construtor of Piece
        /// @param name Name / Type of Piece
        /// @param color Color of Piece
        /// @param rowIdx Row Index
        /// @param colIdx Column Index
        Piece (enum Name name, Chess::Color color, int32_t rowIdx, int8_t colIdx)
        : Name(name), Color(color), Row(rowIdx), Col(colIdx) {}

        /// Construtor of Piece
        /// @param name Name / Type of Piece
        /// @param color Color of Piece
        /// @param rowIdx Row Index
        /// @param colIdx Column Index
        static std::shared_ptr<Piece> Create(enum Name name, Chess::Color color, int8_t rowIdx, int8_t colIdx) {
            return std::make_shared<Piece>(name, color, rowIdx, colIdx);
        }
    };
    
    /// Each block information of Chess
    struct Block {
    public:
        int32_t Row = -1, Col = -1;
        std::shared_ptr<Piece> Piece;
                
        /// Construct the Block instance
        /// @param row row assigned to block
        /// @param col column of block
        /// @param idx block index. from 0 - 63 for row : 2 and Col : 4 , Index will be
        /// row * 8 + col = 20
        static std::shared_ptr<Block> Create(int32_t row, int32_t col) {
            return std::make_shared<Block>(row, col);
        }
        
        Block(int32_t row, int32_t col) : Row(row), Col(col) {}
    };
    
    /// Stores the Power Piece Name (Type) at initial colum position
    /// In start powere Pieces must be at these positions only
    static const enum Piece::Name PowerPiecePosition[MAX_COLUMNS] = {
        Piece::Name::Rook,
        Piece::Name::Knight,
        Piece::Name::Bishop,
        Piece::Name::Queen,
        Piece::Name::King,
        Piece::Name::Bishop,
        Piece::Name::Knight,
        Piece::Name::Rook
    };
    
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
        bool OnKeyPressed(KeyPressedEvent& e);
        bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);
        
        void UpdateHoveredEntity();
        
        void InitBlocksData();
        void InitPlayerData();

        std::shared_ptr<Entity> CreatePieceEntity(const std::string& entityName, std::shared_ptr<Texture> texture, const glm::vec3& position);

        // Member variabls
        Viewport m_ViewportData;
        std::shared_ptr<Scene> m_Scene;
        
        std::shared_ptr<Entity> m_CameraEntity;
        std::shared_ptr<Entity> m_EntityForOutlineHoveredBlock;

        std::shared_ptr<Block> m_Blocks[MAX_ROWS][MAX_COLUMNS];

        Player m_Players[MAX_PLAYER];
    };

}
