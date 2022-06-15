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
        uint32_t WorldLevel = 0;
        uint32_t WorldSubLevel = 0;
        
        PlayerLevel(uint32_t worldLevel = 0, uint32_t worldSubLevel = 0) : WorldLevel(worldLevel), WorldSubLevel(worldSubLevel) {}
    };
    
    
}
