//
//  TextRendering.cpp
//  Mario
//
//  Created by iKan on 15/06/22.
//

#include "TextRendering.hpp"

using namespace Mario;

/// Copy the runtime Text render data
/// @param score Current score of current player
/// @param coinCount Coin count of current player
/// @param worldLevel Current level of player
/// @param worldSubLevel Curretn Sub level of player
/// @param timeLeft Time left for current level
TextRender::Data::Data(const PlayerScore& score, const PlayerLevel& level, uint32_t timeLeft)
: Score(score), Level(level), TimeLeft(timeLeft) { }

/// Render the text after game started
/// @param projection Camera projection
/// @param windowSize window Position
void TextRender::UpdateRunTime(const glm::mat4& projection, const glm::vec2& windowSize, const Data& data) {
    // Score
    Renderer::RenderText("MARIO", projection, glm::vec3(40.0f, windowSize.y - 50.0f, 0.3f), glm::vec2(0.7), { 1.0f, 1.0f, 1.0f, 1.0f });
    Renderer::RenderText(std::to_string(data.Score.TotalScore), projection, glm::vec3(40.0f, windowSize.y - 80.0f, 0.3f), glm::vec2(0.7), { 1.0f, 1.0f, 1.0f, 1.0f });
    
    // Coins
    Renderer::RenderText(std::string(("x") + std::to_string(data.Score.CoinCount)), projection, glm::vec3(windowSize.x / 3.0f, windowSize.y - 80.0f, 0.3f), glm::vec2(0.7), { 1.0f, 1.0f, 1.0f, 1.0f });

    // World Level
    Renderer::RenderText("WORLD", projection, glm::vec3(windowSize.x / 1.5f, windowSize.y - 50.0f, 0.3f), glm::vec2(0.7), { 1.0f, 1.0f, 1.0f, 1.0f });
    std::string levelString = std::to_string(data.Level.WorldLevel) + " - " + std::to_string(data.Level.WorldSubLevel);
    Renderer::RenderText(levelString, projection, glm::vec3(windowSize.x / 1.5f, windowSize.y - 80.0f, 0.3f), glm::vec2(0.7), { 1.0f, 1.0f, 1.0f, 1.0f });

    // Time
    Renderer::RenderText("TIME", projection, glm::vec3(windowSize.x - 120.0, windowSize.y - 50.0f, 0.3f), glm::vec2(0.7), { 1.0f, 1.0f, 1.0f, 1.0f });
    Renderer::RenderText(std::to_string(data.TimeLeft), projection, glm::vec3(windowSize.x - 120.0, windowSize.y - 80.0f, 0.3f), glm::vec2(0.7), { 1.0f, 1.0f, 1.0f, 1.0f });
}

/// Render the text at init time before game started
void TextRender::UpdateInitTime(const glm::mat4& projection, const glm::vec2& windowSize) {
    // Render the Frame rate
    Renderer::RenderText("(c)2022 iKan Mario", projection, glm::vec3(950.0f, 265.0f, 0.3f), glm::vec2(0.5), { 1.0f, 1.0f, 1.0f, 0.8f });

    Renderer::RenderText("1 PLAYER GAME", projection, glm::vec3(windowSize.x / 3.0f, 200.0f, 0.3f), glm::vec2(1.0), { 1.0f, 1.0f, 1.0f, 0.8f });
    Renderer::RenderText("2 PLAYER GAME", projection, glm::vec3(windowSize.x / 3.0f, 150.0f, 0.3f), glm::vec2(1.0), { 1.0f, 1.0f, 1.0f, 0.8f });
}
