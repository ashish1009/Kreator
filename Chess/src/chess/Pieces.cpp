//
//  Piece.cpp
//  Chess
//
//  Created by iKan on 03/06/22.
//

#include "Pieces.hpp"

using namespace Chess;

/// Construtor of Piece
/// @param name Name / Type of Piece
/// @param color Color of Piece
/// @param rowIdx Row Index
/// @param colIdx Column Index
std::shared_ptr<Piece> Piece::Create(enum Piece::Name name, Chess::Color color, int8_t rowIdx, int8_t colIdx) {
    switch (name) {
        case Piece::Name::Pawn : return std::make_shared<Pawn_>(color, rowIdx, colIdx);
        case Piece::Name::King : return std::make_shared<King_>(color, rowIdx, colIdx);
        case Piece::Name::Queen : return std::make_shared<Queen_>(color, rowIdx, colIdx);
        case Piece::Name::Bishop : return std::make_shared<Bishop_>(color, rowIdx, colIdx);
        case Piece::Name::Knight : return std::make_shared<Knight_>(color, rowIdx, colIdx);
        case Piece::Name::Rook : return std::make_shared<Rook_>(color, rowIdx, colIdx);
        default:
            IK_ASSERT(false, "Invalid Piece");       
    }
}

/// Validate the move of Piece at new position
/// @param rowIdx new row position of Piece
/// @param colIdx new row column position of Piece
bool Pawn_::Validate(int8_t rowIdx, int8_t colIdx, bool isNewBlockEmpty) {
    if (isNewBlockEmpty) {
        if (MoveForward) {
            if (Col == colIdx && Row - rowIdx == 1) {
                return true;
            }
        }
        else {
            if (Col == colIdx && rowIdx - Row == 1) {
                return true;
            }
        }
    }
    else {
        if (MoveForward) {
            if (Col - colIdx == 1 && Row - rowIdx == 1) {
                return true;
            }
        }
        else {
            if (colIdx - Col == 1 && rowIdx - Row == 1) {
                return true;
            }
        }
    }
    return false;
}

/// Validate the move of Piece at new position
/// @param rowIdx new row position of Piece
/// @param colIdx new row column position of Piece
bool King_::Validate(int8_t rowIdx, int8_t colIdx, bool isNewBlockEmpty) {
    return false;
}

/// Validate the move of Piece at new position
/// @param rowIdx new row position of Piece
/// @param colIdx new row column position of Piece
bool Queen_::Validate(int8_t rowIdx, int8_t colIdx, bool isNewBlockEmpty) {
    return false;
}

/// Validate the move of Piece at new position
/// @param rowIdx new row position of Piece
/// @param colIdx new row column position of Piece
bool Bishop_::Validate(int8_t rowIdx, int8_t colIdx, bool isNewBlockEmpty) {
    return false;
}

/// Validate the move of Piece at new position
/// @param rowIdx new row position of Piece
/// @param colIdx new row column position of Piece
bool Knight_::Validate(int8_t rowIdx, int8_t colIdx, bool isNewBlockEmpty) {
    return false;
}

/// Validate the move of Piece at new position
/// @param rowIdx new row position of Piece
/// @param colIdx new row column position of Piece
bool Rook_::Validate(int8_t rowIdx, int8_t colIdx, bool isNewBlockEmpty) {
    return false;
}
