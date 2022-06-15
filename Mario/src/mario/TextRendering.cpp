//
//  TextRendering.cpp
//  Mario
//
//  Created by iKan on 15/06/22.
//

#include "TextRendering.hpp"
#include "Background.hpp"

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
void TextRender::UpdateRunTime(const glm::mat4& projection, const Data& data) {
    static glm::vec2 scale = glm::vec3(1.8f);
    static glm::vec4 color = glm::vec4(1.0f);

    glm::vec3 position = glm::vec3(0.3f);

    // Score
    position.x = -15.0f;
    position.y = 7.5f;
    Renderer::RenderText("MARIO", projection, position, scale, color);
    position.y = 6.5f;
    Renderer::RenderText(std::to_string(data.Score.TotalScore), projection, position, scale, color);

    // Coins
    position.x = -6.0f;
    Renderer::RenderText(std::string(("x") + std::to_string(data.Score.CoinCount)), projection, position, scale, color);

    // World Level
    position.x = 3.0f;
    position.y = 7.5f;
    Renderer::RenderText("WORLD", projection, position, scale, color);
    std::string levelString = std::to_string(data.Level.WorldLevel) + " - " + std::to_string(data.Level.WorldSubLevel);
    position.y = 6.5f;
    Renderer::RenderText(levelString, projection, position, scale, color);

    // Time
    position.x = 12.0f;
    position.y = 7.5f;
    Renderer::RenderText("TIME", projection, position, scale, color);
    position.y = 6.5f;
    Renderer::RenderText(std::to_string(data.TimeLeft), projection, position, scale, color);
}

/// Render the text at init time before game started
void TextRender::UpdateInitTime(const glm::mat4& projection) {
    static glm::vec2 scale = glm::vec3(1.8f);
    static glm::vec4 color = glm::vec4(1.0f);

    glm::vec3 position = glm::vec3(0.3f);

    position.x = 5.0f;
    position.y = -2.5f;
    Renderer::RenderText("(c)2022 iKan Mario", projection, position, { 1.2f, 1.2f }, color);

    position.x = -3.0f;
    position.y = -5.0f;
    Renderer::RenderText("1 PLAYER GAME", projection, position, scale, color);
    position.y = -6.0f;
    Renderer::RenderText("2 PLAYER GAME", projection, position, scale, color);
}
