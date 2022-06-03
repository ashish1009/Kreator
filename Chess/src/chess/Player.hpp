//
//  Player.hpp
//  Chess
//
//  Created by iKan on 03/06/22.
//

#pragma once

#include "chess/Common.hpp"

namespace Chess {
    
    /// Player information wrapper
    struct Player {
    public:
        /// Stores the number of player crerted in the game
        static uint32_t NumPlayerCreated;

        // Both Name and Color to be initialized in constructor only;
        std::string Name = "Player";
        Color Color;
        
        /// Construct the Player
        /// @param name Name of player
        Player(const std::string& name) : Name(name) {
            IK_ASSERT(NumPlayerCreated < MAX_PLAYER, "Num Player Reached max");
            /// Assign the color from enum one by one based on index of player
            Color = (Chess::Color)NumPlayerCreated++;
        }
        
    };

}
