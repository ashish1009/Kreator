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
    
    /// Type of Block position
    typedef int8_t POSITION;
    
    // Constants
    static constexpr uint8_t MAX_ROWS = 8;
    static constexpr uint8_t MAX_COLUMNS = 8;
    static constexpr uint8_t MAX_PLAYERS = 2;
    static constexpr uint8_t MAX_PAWNS = 8;
    static constexpr uint8_t MAX_ROOKS = 2;
    static constexpr uint8_t MAX_BISHOP = 2;
    static constexpr uint8_t MAX_KNIGHT = 2;
    
    static constexpr uint8_t PAWN_INIT_ROW_POSITION[2] = { 1, 6 };
    static constexpr uint8_t OTHER_PIECE_INIT_ROW_POSITION[2] = { 0, 7 };
    
    /// Each Piece Data
    struct Piece {
        /// Name of Piece
        enum Name : uint8_t {
            Pawn = 0,
            King = 1,
            Queen = 2,
            Rook = 3,
            Bishop = 4,
            Knight = 5,
            Invalid = 6
        };

        /// Color of Piece
        enum Color : uint8_t {
            Black = 0, White = 1, Max = 2
        };

        /// Position of piece relative to Row and Column
        /// X{ 0 : 7} | Y{ 0 : 7 }
        /// 0 x 0 means Bottom Left
        POSITION m_Row = -1, m_Col = -1;

        /// Name of Piece
        Name m_Name = Name::Invalid;

        /// Color of Piece
        Color m_Color = Color::Max;

        Piece (enum Name name, enum Color color, POSITION x, POSITION y)
        : m_Name(name), m_Color(color), m_Row(x), m_Col(y) {}

        static std::shared_ptr<Piece> Create(enum Name name, enum Color color, POSITION x, POSITION y) {
            return std::make_shared<Piece>(name, color, x, y);
        }
    };
        
    /// Block Data
    class Block {
    public:
        /// Position of block relative to Row and Column
        /// X{ 0 : 7} | Y{ 0 : 7 }
        /// 0 x 0 means Bottom Left
        POSITION m_Row, m_Col;
        
        /// Block Index. e.g. Block at [2][3] is 2 * 8 + 3 = 19
        int8_t m_Index = -1;
        
        /// Store the pice on current blocks
        std::shared_ptr<Piece> m_Piece;
        
        /// Default constructor
        Block() = default;
        
        /// Construct the Block instance
        /// @param row row assigned to block
        /// @param col column of block
        /// @param idx block index. from 0 - 63 for row : 2 and Col : 4 , Index will be
        /// row * 8 + col = 20
        Block(POSITION row, POSITION col) : m_Row(row), m_Col(col) {}
        
        /// Copy the block data
        /// @param other other block data
        Block(const Block& other) : m_Row(other.m_Row), m_Col(other.m_Col), m_Index(other.m_Index) { }
    };

    /// Structure to store Player Information (2 Players in Chess Game)
    struct Player {
        // Methods
        Player(const std::string& name) : m_Name(name) {}

        // Variabls
        std::string m_Name = "Unknown Player";       // Name of Player
        Piece::Color m_Color = Piece::Color::Max;    // Color of Player Piece. By default None is taken

        std::unordered_map<std::shared_ptr<Entity>, std::shared_ptr<Piece>> m_PieceEntityMap; // Stores the Piece data of chess with Entity map
    };

    /// Stores the column position of power piece (other than pawns)
    static const Piece::Name PowerPiecePosition[MAX_COLUMNS] = {
        Piece::Name::Rook,
        Piece::Name::Knight,
        Piece::Name::Bishop,
        Piece::Name::Queen,
        Piece::Name::King,
        Piece::Name::Bishop,
        Piece::Name::Knight,
        Piece::Name::Rook
    };
    
    // TODO: Seprate all date from renderer class to some other class
    
    /// Chess Renderer Layer
    class ChessRendererLayer : public Layer {
    public:
        // TODO: try something else later
        ChessRendererLayer(const std::string& playerName_1, const std::string& playerName_2);
        virtual ~ChessRendererLayer();
        
        void Attach() override;
        void Update(Timestep ts) override;
        void RenderGui() override;
        void Detach() override;
        void EventHandler(Event&) override;
        
        const std::string& GetName() const override { return m_Name; }
        
    private:
        // Member functions
        void InitBlockData();
        void InitPlayerData();
        
        std::shared_ptr<Entity> CreatePieceEntity(const std::string& entityName, std::shared_ptr<Texture> texture, const glm::vec3& position);
        
        bool OnKeyPressed(KeyPressedEvent& e);
        bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);
        
        void UpdateHoveredEntity();
        
        // Member variabls
        Viewport m_ViewportData;                                                // Viewport Data
        std::shared_ptr<Scene> m_Scene;                                         // Stores the Scene
        std::shared_ptr<Entity> m_CameraEntity;                                 // Camera Entity
        std::array<Player, MAX_PLAYERS> m_PlayerData;                           // Stores the Player Data
        
        std::shared_ptr<Block> m_Block[MAX_ROWS][MAX_COLUMNS];
        std::unordered_map<std::shared_ptr<Entity>, std::shared_ptr<Block>> m_BlockEntityMap;    // Stores the blocks data of chess with Entity map
    };

}
