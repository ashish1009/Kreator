//
//  Block.hpp
//  Chess
//
//  Created by iKan on 03/06/22.
//

#pragma once

#include "chess/Common.hpp"
#include "chess/Piece.hpp"

namespace Chess {
    
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

}
