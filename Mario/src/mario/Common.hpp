//
//  Common.h
//  Mario
//
//  Created by iKan on 15/06/22.
//

#pragma once

#include <iKanHeaders.h>

using namespace iKan;

namespace Mario {
    
    /// Store the player score info
    struct PlayerScore {
        uint32_t TotalScore = 0;
        uint32_t CoinCount = 0;
        
        PlayerScore(uint32_t score = 0, uint32_t coin = 0) : TotalScore(score), CoinCount(coin) {}
    };
    
    /// Store the player score info
    struct PlayerLevel {
        uint32_t WorldLevel = 1;
        uint32_t WorldSubLevel = 1;
        
        // By default level will be 1 as game will alwyas start from level 1
        // TODO: Might change later if we want to start the game from specific level
        PlayerLevel(uint32_t worldLevel = 1, uint32_t worldSubLevel = 1) : WorldLevel(worldLevel), WorldSubLevel(worldSubLevel) {}
    };
    
    
}
