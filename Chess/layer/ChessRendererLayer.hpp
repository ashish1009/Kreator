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
        
    /// Block Data
    class Block {
    public:
        /// Position of block relative to Row and Column
        /// X{ 0 : 7} | Y{ 0 : 7 }
        /// 0 x 0 means Bottom Left
        POSITION m_Row, m_Col;
        
        /// Block Index. e.g. Block at [2][3] is 2 * 8 + 3 = 19
        int8_t m_Index = -1;
        
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

    /// Structure to store Player Information (2 Players in Chess Game)
    struct Player {
        // Methods
        Player(const std::string& name) : m_Name(name) {}

        // Variabls
        std::string m_Name = "Unknown Player";       // Name of Player
        Piece::Color m_Color = Piece::Color::Max;    // Color of Player Piece. By default None is taken

        std::unordered_map<std::shared_ptr<Entity>, std::shared_ptr<Piece>> m_PieceEntityMap; // Stores the Piece data of chess with Entity map

        // Piece Data
        std::vector<std::shared_ptr<Piece>> m_Pawns;  // Vector of all Pawns
        std::vector<std::shared_ptr<Piece>> m_Rook;   // Vector of both Rook
        std::vector<std::shared_ptr<Piece>> m_Bishop; // Vector of both Bishop
        std::vector<std::shared_ptr<Piece>> m_Knight; // Vector of both Knight

        std::shared_ptr<Piece> m_King;
        std::shared_ptr<Piece> m_Queen;
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
        
        bool OnKeyPressed(KeyPressedEvent& e);
        bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);
        
        void UpdateHoveredEntity();
        
        // Member variabls
        Viewport m_ViewportData;                                                // Viewport Data
        std::shared_ptr<Scene> m_Scene;                                         // Stores the Scene
        std::shared_ptr<Entity> m_CameraEntity;                                 // Camera Entity
        std::unordered_map<std::shared_ptr<Entity>, Block> m_BlockEntityMap;    // Stores the blocks data of chess with Entity map
        std::array<Player, MAX_PLAYERS> m_PlayerData;                           // Stores the Player Data
        
        // Texture Constants
        // TODO: move some other place.
        struct PieceTexture {
            // Pawns
            const std::shared_ptr<Texture> c_BlackPawn = Texture::Create(AssetManager::GetClientAsset("/texture/black/pawn.png"));
            const std::shared_ptr<Texture> c_WhitePawn = Texture::Create(AssetManager::GetClientAsset("/texture/white/pawn.png"));

            // Whites
            const std::shared_ptr<Texture> c_WhiteKing = Texture::Create(AssetManager::GetClientAsset("/texture/white/king.png"));
            const std::shared_ptr<Texture> c_WhiteQuuen = Texture::Create(AssetManager::GetClientAsset("/texture/white/queen.png"));
            const std::shared_ptr<Texture> c_WhiteRook = Texture::Create(AssetManager::GetClientAsset("/texture/white/rook.png"));
            const std::shared_ptr<Texture> c_WhiteBishop = Texture::Create(AssetManager::GetClientAsset("/texture/white/bishop.png"));
            const std::shared_ptr<Texture> c_WhiteKnight = Texture::Create(AssetManager::GetClientAsset("/texture/white/knight.png"));

            // Blacks
            const std::shared_ptr<Texture> c_BlackKing = Texture::Create(AssetManager::GetClientAsset("/texture/black/king.png"));
            const std::shared_ptr<Texture> c_BlackQuuen = Texture::Create(AssetManager::GetClientAsset("/texture/black/queen.png"));
            const std::shared_ptr<Texture> c_BlackRook = Texture::Create(AssetManager::GetClientAsset("/texture/black/rook.png"));
            const std::shared_ptr<Texture> c_BlackBishop = Texture::Create(AssetManager::GetClientAsset("/texture/black/bishop.png"));
            const std::shared_ptr<Texture> c_BlackKnight = Texture::Create(AssetManager::GetClientAsset("/texture/black/knight.png"));
        };
        PieceTexture m_PieceTexture;
    };

}
