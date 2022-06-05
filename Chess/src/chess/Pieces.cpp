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

/// Update the position of Piece
/// @param rowIdx new Row
/// @param colIdx new Column
void Piece::UpdatePosition(int8_t rowIdx, int8_t colIdx) {
    Row = rowIdx;
    Col = colIdx;
    auto& position = Entity->GetComponent<TransformComponent>().Translation;
    position = { Col, Row, 0.1f };
}


/// Validate the move of Piece at new position
/// @param rowIdx new row position of Piece
/// @param colIdx new row column position of Piece
bool Pawn_::ValidateAndUpdatePostion(int8_t rowIdx, int8_t colIdx, bool isNewBlockEmpty) {
    if (isNewBlockEmpty) {
        if (MoveForward) {
            if (Col == colIdx && rowIdx - Row == 1) {
                UpdatePosition(rowIdx, colIdx);
                return true;
            }
        }
        else {
            if (Col == colIdx && Row - rowIdx == 1) {
                UpdatePosition(rowIdx, colIdx);
                return true;
            }
        }
    }
    else {
        if (MoveForward) {
            if (abs(Col - colIdx) == 1 && rowIdx - Row == 1) {
                UpdatePosition(rowIdx, colIdx);
                return true;
            }
        }
        else {
            if (abs(colIdx - Col) == 1 && Row - rowIdx == 1) {
                UpdatePosition(rowIdx, colIdx);
                return true;
            }
        }
    }
    return false;
}

/// Validate the move of Piece at new position
/// @param rowIdx new row position of Piece
/// @param colIdx new row column position of Piece
bool King_::ValidateAndUpdatePostion(int8_t rowIdx, int8_t colIdx, bool isNewBlockEmpty) {
    return false;
}

/// Validate the move of Piece at new position
/// @param rowIdx new row position of Piece
/// @param colIdx new row column position of Piece
bool Queen_::ValidateAndUpdatePostion(int8_t rowIdx, int8_t colIdx, bool isNewBlockEmpty) {
    return false;
}

/// Validate the move of Piece at new position
/// @param rowIdx new row position of Piece
/// @param colIdx new row column position of Piece
bool Bishop_::ValidateAndUpdatePostion(int8_t rowIdx, int8_t colIdx, bool isNewBlockEmpty) {
    return false;
}

/// Validate the move of Piece at new position
/// @param rowIdx new row position of Piece
/// @param colIdx new row column position of Piece
bool Knight_::ValidateAndUpdatePostion(int8_t rowIdx, int8_t colIdx, bool isNewBlockEmpty) {
    if ((abs(Row - rowIdx) == 2 && abs(Col - colIdx) == 1) || (abs(Row - rowIdx) == 1 && abs(Col - colIdx) == 2)) {
        UpdatePosition(rowIdx, colIdx);
        return true;
    }
    return false;
}

/// Validate the move of Piece at new position
/// @param rowIdx new row position of Piece
/// @param colIdx new row column position of Piece
bool Rook_::ValidateAndUpdatePostion(int8_t rowIdx, int8_t colIdx, bool isNewBlockEmpty) {
    return false;
}
