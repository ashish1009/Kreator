//
//  Piece.hpp
//  Chess
//
//  Created by iKan on 03/06/22.
//

#pragma once

#include "chess/Common.hpp"

namespace Chess {
    
    // Max number of pawns at the init time fo each player
    static constexpr uint8_t MAX_PAWNS = 8;
    
    // Row Index of Pawns at start of game for each player
    static constexpr uint8_t PAWN_INIT_ROW_POSITION[MAX_PLAYER] = { 1, 6 };
    // Row Index of Power piece at start of game for each player
    static constexpr uint8_t POWER_PIECE_INIT_ROW_POSITION[MAX_PLAYER] = { 0, 7 };
    
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
        
        virtual ~Piece() = default;
        
        /// Validate the move of Piece at new position
        /// @param rowIdx new row position of Piece
        /// @param colIdx new row column position of Piece
        virtual bool Validate(int8_t rowIdx, int8_t colIdx) = 0;

        /// Construtor of Piece
        /// @param name Name / Type of Piece
        /// @param color Color of Piece
        /// @param rowIdx Row Index
        /// @param colIdx Column Index
        static std::shared_ptr<Piece> Create(enum Name name, Chess::Color color, int8_t rowIdx, int8_t colIdx);
    };
    
    /// Pawn class derived from base Piece
    struct Pawn_ : public Piece {
    public:
        /// Construtor of Pawn
        /// @param color Color of Piece
        /// @param rowIdx Row Index
        /// @param colIdx Column Index
        Pawn_ (Chess::Color color, int32_t rowIdx, int8_t colIdx)
        : Piece(Piece::Name::Pawn, color, rowIdx, colIdx) {}
        
        /// Validate the move of Piece at new position
        /// @param rowIdx new row position of Piece
        /// @param colIdx new row column position of Piece
        bool Validate(int8_t rowIdx, int8_t colIdx) override;
    };
    
    /// King class derived from base Piece
    struct King_ : public Piece {
    public:
        /// Construtor of King
        /// @param color Color of Piece
        /// @param rowIdx Row Index
        /// @param colIdx Column Index
        King_ (Chess::Color color, int32_t rowIdx, int8_t colIdx)
        : Piece(Piece::Name::King, color, rowIdx, colIdx) {}

        /// Validate the move of Piece at new position
        /// @param rowIdx new row position of Piece
        /// @param colIdx new row column position of Piece
        bool Validate(int8_t rowIdx, int8_t colIdx) override;
    };

    /// Queen class derived from base Piece
    struct Queen_ : public Piece {
    public:
        /// Construtor of Queen
        /// @param color Color of Piece
        /// @param rowIdx Row Index
        /// @param colIdx Column Index
        Queen_ (Chess::Color color, int32_t rowIdx, int8_t colIdx)
        : Piece(Piece::Name::Queen, color, rowIdx, colIdx) {}

        /// Validate the move of Piece at new position
        /// @param rowIdx new row position of Piece
        /// @param colIdx new row column position of Piece
        bool Validate(int8_t rowIdx, int8_t colIdx) override;
    };

    /// Bishop class derived from base Piece
    struct Bishop_ : public Piece {
    public:
        /// Construtor of Bishop
        /// @param color Color of Piece
        /// @param rowIdx Row Index
        /// @param colIdx Column Index
        Bishop_ (Chess::Color color, int32_t rowIdx, int8_t colIdx)
        : Piece(Piece::Name::Bishop, color, rowIdx, colIdx) {}

        /// Validate the move of Piece at new position
        /// @param rowIdx new row position of Piece
        /// @param colIdx new row column position of Piece
        bool Validate(int8_t rowIdx, int8_t colIdx) override;
    };

    /// Knight class derived from base Piece
    struct Knight_ : public Piece {
    public:
        /// Construtor of Knight
        /// @param color Color of Piece
        /// @param rowIdx Row Index
        /// @param colIdx Column Index
        Knight_ (Chess::Color color, int32_t rowIdx, int8_t colIdx)
        : Piece(Piece::Name::Knight, color, rowIdx, colIdx) {}

        /// Validate the move of Piece at new position
        /// @param rowIdx new row position of Piece
        /// @param colIdx new row column position of Piece
        bool Validate(int8_t rowIdx, int8_t colIdx) override;
    };

    /// Rook class derived from base Piece
    struct Rook_ : public Piece {
    public:
        /// Construtor of Rook
        /// @param color Color of Piece
        /// @param rowIdx Row Index
        /// @param colIdx Column Index
        Rook_ (Chess::Color color, int32_t rowIdx, int8_t colIdx)
        : Piece(Piece::Name::Rook, color, rowIdx, colIdx) {}

        /// Validate the move of Piece at new position
        /// @param rowIdx new row position of Piece
        /// @param colIdx new row column position of Piece
        bool Validate(int8_t rowIdx, int8_t colIdx) override;
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
    
}
