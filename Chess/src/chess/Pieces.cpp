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
bool Pawn_::Validate(int8_t rowIdx, int8_t colIdx, bool isNewBlockEmpty) {
    if (isNewBlockEmpty) {
        if (MoveForward) {
            if (Col == colIdx && rowIdx - Row == 1)
                return true;
        }
        else {
            if (Col == colIdx && Row - rowIdx == 1)
                return true;
        }
    }
    else {
        if (MoveForward) {
            if (abs(Col - colIdx) == 1 && rowIdx - Row == 1)
                return true;
        }
        else {
            if (abs(colIdx - Col) == 1 && Row - rowIdx == 1)
                return true;
        }
    }
    return false;
}

/// Get the possible block postion where block can be moved
std::vector<BLOCK_ROW_COL> Pawn_::GetPossibleMovePosition() const {
    std::vector<BLOCK_ROW_COL> result;
    // Max possible move for pawn is 3
    if (MoveForward) {
        // Diagonal Left
        if (Col > 0)
            result.emplace_back(std::make_pair(Row + 1, Col - 1));
        // Diagonal Right
        if (Col < MAX_COLUMNS)
            result.emplace_back(std::make_pair(Row + 1, Col + 1));
        // Straight
        result.emplace_back(std::make_pair(Row + 1, Col));
    }
    else {
        // Diagonal Left
        if (Col > 0)
            result.emplace_back(std::make_pair(Row - 1, Col - 1));
        // Diagonal Right
        if (Col < MAX_COLUMNS)
            result.emplace_back(std::make_pair(Row - 1, Col + 1));
        // Straight
        result.emplace_back(std::make_pair(Row - 1, Col));
    }
    return result;
}

/// Validate the move of Piece at new position
/// @param rowIdx new row position of Piece
/// @param colIdx new row column position of Piece
bool King_::Validate(int8_t rowIdx, int8_t colIdx, bool isNewBlockEmpty) {
    return false;
}

/// Get the possible block postion where block can be moved
std::vector<BLOCK_ROW_COL> King_::GetPossibleMovePosition() const {
    std::vector<BLOCK_ROW_COL> result;
    return result;
}

/// Validate the move of Piece at new position
/// @param rowIdx new row position of Piece
/// @param colIdx new row column position of Piece
bool Queen_::Validate(int8_t rowIdx, int8_t colIdx, bool isNewBlockEmpty) {
    return false;
}

/// Get the possible block postion where block can be moved
std::vector<BLOCK_ROW_COL> Queen_::GetPossibleMovePosition() const {
    std::vector<BLOCK_ROW_COL> result;
    return result;
}

/// Validate the move of Piece at new position
/// @param rowIdx new row position of Piece
/// @param colIdx new row column position of Piece
bool Bishop_::Validate(int8_t rowIdx, int8_t colIdx, bool isNewBlockEmpty) {
    return false;
}

/// Get the possible block postion where block can be moved
std::vector<BLOCK_ROW_COL> Bishop_::GetPossibleMovePosition() const {
    std::vector<BLOCK_ROW_COL> result;
    return result;
}

/// Validate the move of Piece at new position
/// @param rowIdx new row position of Piece
/// @param colIdx new row column position of Piece
bool Knight_::Validate(int8_t rowIdx, int8_t colIdx, bool isNewBlockEmpty) {
    if ((abs(Row - rowIdx) == 2 && abs(Col - colIdx) == 1) || (abs(Row - rowIdx) == 1 && abs(Col - colIdx) == 2))
        return true;

    return false;
}

/// Get the possible block postion where block can be moved
std::vector<BLOCK_ROW_COL> Knight_::GetPossibleMovePosition() const {
    std::vector<BLOCK_ROW_COL> result;
    return result;
}

/// Validate the move of Piece at new position
/// @param rowIdx new row position of Piece
/// @param colIdx new row column position of Piece
bool Rook_::Validate(int8_t rowIdx, int8_t colIdx, bool isNewBlockEmpty) {
    return false;
}

/// Get the possible block postion where block can be moved
std::vector<BLOCK_ROW_COL> Rook_::GetPossibleMovePosition() const {
    std::vector<BLOCK_ROW_COL> result;
    return result;
}

bool Piece::ValidateAndUpdatePostion(int8_t rowIdx, int8_t colIdx, bool isNewBlockEmpty) {
    if (Validate(rowIdx, colIdx, isNewBlockEmpty)) {
        UpdatePosition(rowIdx, colIdx);
        return true;
    }
    return false;
}
