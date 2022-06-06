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
bool Pawn_::Validate(int8_t rowIdx, int8_t colIdx, std::shared_ptr<Block> blocks[MAX_ROWS][MAX_COLUMNS]) {
    if (rowIdx < 0 || rowIdx > MAX_ROWS || colIdx < 0 || colIdx > MAX_COLUMNS)
        return false;
    
    if (!blocks[rowIdx][colIdx]->Piece) {
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
bool King_::Validate(int8_t rowIdx, int8_t colIdx, std::shared_ptr<Block> blocks[MAX_ROWS][MAX_COLUMNS]) {
    if (rowIdx < 0 || rowIdx > MAX_ROWS || colIdx < 0 || colIdx > MAX_COLUMNS)
        return false;
    
    if (abs(Col - colIdx) == 1 && abs(rowIdx - Row) == 1)
        return true;

    if (Col == colIdx && abs(rowIdx - Row) == 1)
        return true;

    if (abs(Col - colIdx) == 1 && rowIdx == Row)
        return true;
    
    return false;
}

/// Get the possible block postion where block can be moved
std::vector<BLOCK_ROW_COL> King_::GetPossibleMovePosition() const {
    std::vector<BLOCK_ROW_COL> result;
    if (Row < MAX_ROWS - 1) {
        if (Col > 0)
            result.emplace_back(std::make_pair(Row + 1, Col - 1));
        if (Col < MAX_COLUMNS - 1)
            result.emplace_back(std::make_pair(Row + 1, Col + 1));
        result.emplace_back(std::make_pair(Row + 1, Col));
    }
    if (Row > 0) {
        if (Col > 0)
            result.emplace_back(std::make_pair(Row - 1, Col - 1));
        if (Col < MAX_COLUMNS - 1)
            result.emplace_back(std::make_pair(Row - 1, Col + 1));
        result.emplace_back(std::make_pair(Row - 1, Col));
    }
    
    if (Col > 0)
        result.emplace_back(std::make_pair(Row, Col - 1));
    if (Col < MAX_COLUMNS - 1)
        result.emplace_back(std::make_pair(Row, Col + 1));

    return result;
}

/// Validate the move of Piece at new position
/// @param rowIdx new row position of Piece
/// @param colIdx new row column position of Piece
bool Queen_::Validate(int8_t rowIdx, int8_t colIdx, std::shared_ptr<Block> blocks[MAX_ROWS][MAX_COLUMNS]) {
    if (rowIdx < 0 || rowIdx > MAX_ROWS || colIdx < 0 || colIdx > MAX_COLUMNS)
        return false;
    
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
bool Bishop_::Validate(int8_t rowIdx, int8_t colIdx, std::shared_ptr<Block> blocks[MAX_ROWS][MAX_COLUMNS]) {
    if (rowIdx < 0 || rowIdx > MAX_ROWS || colIdx < 0 || colIdx > MAX_COLUMNS)
        return false;
    
    // Check is destination is diagonal or not?
    if (abs(Row - rowIdx) != abs(Col - colIdx))
        return false;
    
    const auto& destBlock = blocks[rowIdx][colIdx];
    if (Row > rowIdx) rowIdx++; else rowIdx--;
    if (Col > colIdx) colIdx++; else colIdx--;

    // Do either for Row or colum as they iterate same in diagonal
    while (abs(Row - rowIdx) >= 1) {
        // Update the pointer at new block based on the value of desitation Block
        if (blocks[rowIdx][colIdx]->Piece)
            break;        

        if (Row > rowIdx) rowIdx++; else rowIdx--;
        if (Col > colIdx) colIdx++; else colIdx--;
    }
    
    // If No piece is found in path
    if (rowIdx == Row) {
        // Check the destination block in the end
        if (destBlock->Piece && destBlock->Piece->Color == Color)
            return false;
        else
            return true;
    }
        
    return false;
    
}

/// Get the possible block postion where block can be moved
std::vector<BLOCK_ROW_COL> Bishop_::GetPossibleMovePosition() const {
    std::vector<BLOCK_ROW_COL> result;
    int8_t row, col;
    // Top Right
    if (Col < MAX_COLUMNS - 1 && Row < MAX_ROWS - 1) {
        row = Row + 1; col = Col + 1;
        while (row < MAX_ROWS && col < MAX_COLUMNS)
            result.emplace_back(std::make_pair(row++, col++));
    }
    
    // Top Left
    if (Col > 0 && Row < MAX_ROWS - 1) {
        row = Row + 1; col = Col - 1;
        while (row < MAX_ROWS && col >= 0)
            result.emplace_back(std::make_pair(row++, col--));
    }
    
    // Bottom Right
    if (Col < MAX_COLUMNS - 1 && Row > 0) {
        row = Row - 1; col = Col + 1;
        while (row >=0 && col < MAX_COLUMNS)
            result.emplace_back(std::make_pair(row--, col++));
    }
    
    // Bottom Left
    if (Col > 0 && Row > 0) {
        row = Row - 1; col = Col - 1;
        while (row >= 0 && col >= 0)
            result.emplace_back(std::make_pair(row--, col--));
    }
    
    return result;
}

/// Validate the move of Piece at new position
/// @param rowIdx new row position of Piece
/// @param colIdx new row column position of Piece
bool Knight_::Validate(int8_t rowIdx, int8_t colIdx, std::shared_ptr<Block> blocks[MAX_ROWS][MAX_COLUMNS]) {
    if (rowIdx < 0 || rowIdx > MAX_ROWS || colIdx < 0 || colIdx > MAX_COLUMNS)
        return false;
    
    if ((abs(Row - rowIdx) == 2 && abs(Col - colIdx) == 1) || (abs(Row - rowIdx) == 1 && abs(Col - colIdx) == 2))
        return true;

    return false;
}

/// Get the possible block postion where block can be moved
std::vector<BLOCK_ROW_COL> Knight_::GetPossibleMovePosition() const {
    std::vector<BLOCK_ROW_COL> result;
    if (Row > 1) {
        if (Col > 0)
            result.emplace_back(std::make_pair(Row - 2, Col - 1));
        if (Col < MAX_COLUMNS - 1)
            result.emplace_back(std::make_pair(Row - 2, Col + 1));
    }
    if (Row < MAX_ROWS - 2) {
        if (Col > 0)
            result.emplace_back(std::make_pair(Row + 2, Col - 1));
        if (Col < MAX_COLUMNS - 1)
            result.emplace_back(std::make_pair(Row + 2, Col + 1));
    }

    if (Col > 1) {
        if (Row > 0)
            result.emplace_back(std::make_pair(Row - 1, Col - 2));
        if (Row < MAX_ROWS - 1)
            result.emplace_back(std::make_pair(Row + 1, Col - 2));
    }
    if (Col < MAX_COLUMNS - 2) {
        if (Row > 0)
            result.emplace_back(std::make_pair(Row - 1, Col + 2));
        if (Row < MAX_ROWS - 1)
            result.emplace_back(std::make_pair(Row + 1, Col + 2));
    }
    return result;
}

/// Validate the move of Piece at new position
/// @param rowIdx new row position of Piece
/// @param colIdx new row column position of Piece
bool Rook_::Validate(int8_t rowIdx, int8_t colIdx, std::shared_ptr<Block> blocks[MAX_ROWS][MAX_COLUMNS]) {
    if (rowIdx < 0 || rowIdx > MAX_ROWS || colIdx < 0 || colIdx > MAX_COLUMNS)
        return false;
    
    return false;
}

/// Get the possible block postion where block can be moved
std::vector<BLOCK_ROW_COL> Rook_::GetPossibleMovePosition() const {
    std::vector<BLOCK_ROW_COL> result;
    return result;
}

bool Piece::ValidateAndUpdatePostion(int8_t rowIdx, int8_t colIdx, std::shared_ptr<Block> blocks[MAX_ROWS][MAX_COLUMNS]) {
    if (Validate(rowIdx, colIdx, blocks)) {
        UpdatePosition(rowIdx, colIdx);
        return true;
    }
    return false;
}
