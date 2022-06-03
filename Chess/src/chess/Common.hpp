//
//  Common.hpp
//  Chess
//
//  Created by iKan on 03/06/22.
//

#pragma once

#include <iKanHeaders.h>

namespace Chess {
    
    /// Color of player and Pieces
    /// NOTE: each color is for a player unique so last element is MAX Player
    /// as number of color can never exceed Number of player
    enum Color { Black = 0, White = 1, MAX_PLAYER = 2 };
    
    // Constants
    static constexpr uint8_t MAX_ROWS = 8;
    static constexpr uint8_t MAX_COLUMNS = 8;

}
