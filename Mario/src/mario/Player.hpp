//
//  Player.hpp
//  Mario
//
//  Created by iKan on 15/06/22.
//

#pragma once

#include <iKanHeaders.h>
#include "mario/Common.hpp"

using namespace iKan;

namespace Mario {
    
    /// Store the Player and its controllers
    class Player {
    public:
        Player() = default;
        ~Player() = default;
        
        /// return the Score
        const PlayerScore& GetScore();
        const PlayerLevel& GetLevel();
        
    private:
        PlayerScore m_Score;
        PlayerLevel m_Level;
    };
    
}
