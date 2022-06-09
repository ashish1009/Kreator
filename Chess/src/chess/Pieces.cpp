//
//  Piece.cpp
//  Chess
//
//  Created by iKan on 03/06/22.
//

#include "Pieces.hpp"

using namespace Chess;

/// validate the diagonal move of piece
/// @param row source row of Piece
/// @param col source column of piece
/// @param rowIdx destination row of piece
/// @param colIdx destination column of piece
static bool ValidateDiagonalMove(int8_t srcRow, int8_t srcCol, int8_t dstRow, int8_t dstCol, std::shared_ptr<Block> blocks[MAX_ROWS][MAX_COLUMNS]) {
    if (dstRow < 0 || dstRow > MAX_ROWS || dstCol < 0 || dstCol > MAX_COLUMNS)
        return false;

    // Check is destination is diagonal or not?
    if (abs(srcRow - dstRow) != abs(srcCol - dstCol))
        return false;

    // Do either for Row or colum as they iterate same in diagonal
    while (abs(srcRow - dstRow) >= 1) {
        // Update the pointer at new block based on the value of desitation Block
        if (srcRow > dstRow) dstRow++; else dstRow--;
        if (srcCol > dstCol) dstCol++; else dstCol--;

        // if we reached the source block without any obstacle in path then return true
        if (srcRow == dstRow)
            return true;
        
        // if any obstacle found in path return false
        if (blocks[dstRow][dstCol]->Piece)
            return false;
    }
    return true;
}

/// validate the Straight move of piece
/// @param row source row of Piece
/// @param col source column of piece
/// @param rowIdx destination row of piece
/// @param colIdx destination column of piece
static bool ValidateStraightMove(int8_t srcRow, int8_t srcCol, int8_t dstRow, int8_t dstCol, std::shared_ptr<Block> blocks[MAX_ROWS][MAX_COLUMNS]) {
    if (dstRow < 0 || dstRow > MAX_ROWS || dstCol < 0 || dstCol > MAX_COLUMNS)
        return false;

    if (srcRow == dstRow) {
        // Do either for Row or colum as they iterate same in diagonal
        while (dstCol < MAX_COLUMNS && dstCol >= 0) {
            // Update the pointer at new block based on the value of desitation Block
            if (srcCol > dstCol) dstCol++; else dstCol--;

            // if we reached the source block without any obstacle in path then return true
            if (srcCol == dstCol)
                return true;
            
            // if any obstacle found in path return false
            if (blocks[dstRow][dstCol]->Piece)
                return false;
        }
    }
    
    if (srcCol == dstCol) {
        // Do either for Row or colum as they iterate same in diagonal
        while (dstRow < MAX_ROWS && dstRow >= 0) {
            // Update the pointer at new block based on the value of desitation Block
            if (srcRow > dstRow) dstRow++; else dstRow--;

            // if we reached the source block without any obstacle in path then return true
            if (srcRow == dstRow)
                return true;
            
            // if any obstacle found in path return false
            if (blocks[dstRow][dstCol]->Piece)
                return false;
        }
    }
    return false;
}

/// store the possible diagonal move from source
/// @param row source row of Piece
/// @param col source column of piece
/// @param possibleBlocks reference of vector where we will store the possible move block
static void PossibleDiagonalMoveBlock(int8_t srcRow, int8_t srcCol, std::vector<BLOCK_ROW_COL>& possibleBlocks) {
    int8_t row, col;
    // Top Right
    if (srcCol < MAX_COLUMNS - 1 && srcRow < MAX_ROWS - 1) {
        row = srcRow + 1; col = srcCol + 1;
        while (row < MAX_ROWS && col < MAX_COLUMNS)
            possibleBlocks.emplace_back(std::make_pair(row++, col++));
    }
    
    // Top Left
    if (srcCol > 0 && srcRow < MAX_ROWS - 1) {
        row = srcRow + 1; col = srcCol - 1;
        while (row < MAX_ROWS && col >= 0)
            possibleBlocks.emplace_back(std::make_pair(row++, col--));
    }
    
    // Bottom Right
    if (srcCol < MAX_COLUMNS - 1 && srcRow > 0) {
        row = srcRow - 1; col = srcCol + 1;
        while (row >=0 && col < MAX_COLUMNS)
            possibleBlocks.emplace_back(std::make_pair(row--, col++));
    }
    
    // Bottom Left
    if (srcCol > 0 && srcRow > 0) {
        row = srcRow - 1; col = srcCol - 1;
        while (row >= 0 && col >= 0)
            possibleBlocks.emplace_back(std::make_pair(row--, col--));
    }
}

/// store the possible Straight move from source
/// @param row source row of Piece
/// @param col source column of piece
/// @param possibleBlocks reference of vector where we will store the possible move block
static void PossibleStraightMoveBlock(int8_t srcRow, int8_t srcCol, std::vector<BLOCK_ROW_COL>& possibleBlocks) {
    int8_t row, col;
    // Right
    if (srcCol < MAX_COLUMNS - 1) {
        col = srcCol + 1;
        while (col < MAX_COLUMNS)
            possibleBlocks.emplace_back(std::make_pair(srcRow, col++));
    }
    
    // Left
    if (srcCol > 0) {
        col = srcCol - 1;
        while (col >= 0)
            possibleBlocks.emplace_back(std::make_pair(srcRow, col--));
    }
    
    // Bottom
    if (srcRow > 0) {
        row = srcRow - 1;
        while (row >= 0)
            possibleBlocks.emplace_back(std::make_pair(row--, srcCol));
    }
    
    // Top
    if (srcRow < MAX_ROWS - 1) {
        row = srcRow + 1;
        while (row < MAX_ROWS)
            possibleBlocks.emplace_back(std::make_pair(row++, srcCol));
    }
}

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
bool Pawn_::Validate(int8_t rowIdx, int8_t colIdx, std::shared_ptr<Block> blocks[MAX_ROWS][MAX_COLUMNS]) {\
    if (abs(Row - rowIdx) > 1 || abs(Col - colIdx) > 1)
        return false;
    
    // if destination block is EMPTY
    if (!blocks[rowIdx][colIdx]->Piece) {
        return ValidateStraightMove(Row, Col, rowIdx, colIdx, blocks);
    }
    // if destination block is NOT Empty
    else {
        const auto& destBlock = blocks[rowIdx][colIdx];
        if (ValidateDiagonalMove(Row, Col, rowIdx, colIdx, blocks)) {
            // If No piece is found in path
            // Check the destination block in the end
            return (!(destBlock->Piece && destBlock->Piece->Color == Color));
        }
        return false;
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
    if (abs(Row - rowIdx) > 1 || abs(Col - colIdx) > 1)
        return false;

    return ValidateDiagonalMove(Row, Col, rowIdx, colIdx, blocks) | ValidateStraightMove(Row, Col, rowIdx, colIdx, blocks);
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
    return ValidateDiagonalMove(Row, Col, rowIdx, colIdx, blocks) | ValidateStraightMove(Row, Col, rowIdx, colIdx, blocks);
}

/// Get the possible block postion where block can be moved
std::vector<BLOCK_ROW_COL> Queen_::GetPossibleMovePosition() const {
    std::vector<BLOCK_ROW_COL> result;
    PossibleDiagonalMoveBlock(Row, Col, result);
    PossibleStraightMoveBlock(Row, Col, result);
    return result;
}

/// Validate the move of Piece at new position
/// @param rowIdx new row position of Piece
/// @param colIdx new row column position of Piece
bool Bishop_::Validate(int8_t rowIdx, int8_t colIdx, std::shared_ptr<Block> blocks[MAX_ROWS][MAX_COLUMNS]) {
    const auto& destBlock = blocks[rowIdx][colIdx];
    if (ValidateDiagonalMove(Row, Col, rowIdx, colIdx, blocks)) {
        // If No piece is found in path
        // Check the destination block in the end
        return (!(destBlock->Piece && destBlock->Piece->Color == Color));
    }
    return false;
}

/// Get the possible block postion where block can be moved
std::vector<BLOCK_ROW_COL> Bishop_::GetPossibleMovePosition() const {
    std::vector<BLOCK_ROW_COL> result;
    PossibleDiagonalMoveBlock(Row, Col, result);
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
    const auto& destBlock = blocks[rowIdx][colIdx];
    if (ValidateStraightMove(Row, Col, rowIdx, colIdx, blocks)) {
        // If No piece is found in path
        // Check the destination block in the end
        return (!(destBlock->Piece && destBlock->Piece->Color == Color));
    }
    return false;
}

/// Get the possible block postion where block can be moved
std::vector<BLOCK_ROW_COL> Rook_::GetPossibleMovePosition() const {
    std::vector<BLOCK_ROW_COL> result;
    PossibleStraightMoveBlock(Row, Col, result);
    return result;
}

bool Piece::ValidateAndUpdatePostion(int8_t rowIdx, int8_t colIdx, std::shared_ptr<Block> blocks[MAX_ROWS][MAX_COLUMNS]) {
    if (Validate(rowIdx, colIdx, blocks)) {
        UpdatePosition(rowIdx, colIdx);
        return true;
    }
    return false;
}
