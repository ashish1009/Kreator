//
//  Player.cpp
//  Mario
//
//  Created by iKan on 15/06/22.
//

#include "Player.hpp"

using namespace Mario;

const PlayerScore& Player::GetScore() { return m_Score; }
const PlayerLevel& Player::GetLevel() { return m_Level; }
