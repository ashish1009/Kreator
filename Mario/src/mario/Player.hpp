//
//  Player.hpp
//  Mario
//
//  Created by iKan on 15/06/22.
//

#pragma once

#include <iKanHeaders.h>
using namespace iKan;

namespace Mario {
    
    /// Store the Player and its controllers
    class Player {
    public:
        struct Score {
            uint32_t TotalScore = 0;
            uint32_t CoinCount = 0;
        };
        
        Player() = default;
        ~Player() = default;
        
        /// return the Score
        const Score& GetScore();
        
    private:
        Score m_Score;
    };
    
}
