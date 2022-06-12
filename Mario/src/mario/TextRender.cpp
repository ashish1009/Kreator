//
//  TextRender.cpp
//  Mario
//
//  Created by iKan on 12/06/22.
//

#include "TextRender.hpp"

using namespace Mario;

struct TextRenderData {
    uint32_t Score;
    uint32_t CoinCount;
    
    uint32_t WorldLevel;
    uint32_t WorldSubLevel;
    
    uint32_t TimeLeft;
};
static TextRenderData s_Data;

/// Initialize the Text Data
void TextRender::Init(uint32_t worldLevel, uint32_t worldSubLevel, uint32_t timeLeft) {
    s_Data.Score = 0;
    s_Data.CoinCount = 0;
    s_Data.WorldLevel = worldLevel;
    s_Data.WorldSubLevel = worldSubLevel;
    s_Data.TimeLeft = timeLeft;
}

/// Update the Teext rendere for Mario
/// @param projection Camera projection
/// @param windowSize window Position
void TextRender::Update(const glm::mat4& projection, const glm::vec2& windowSize) {
    // Score
    Renderer::RenderText("MARIO", projection, glm::vec3(40.0f, windowSize.y - 50.0f, 0.3f), glm::vec2(0.7), { 1.0f, 1.0f, 1.0f, 1.0f });
    Renderer::RenderText(std::to_string(s_Data.Score), projection, glm::vec3(40.0f, windowSize.y - 80.0f, 0.3f), glm::vec2(0.7), { 1.0f, 1.0f, 1.0f, 1.0f });
    
    // Coins
    Renderer::RenderText(std::string(("x") + std::to_string(s_Data.CoinCount)), projection, glm::vec3(500.0f, windowSize.y - 80.0f, 0.3f), glm::vec2(0.7), { 1.0f, 1.0f, 1.0f, 1.0f });

    // World Level
    Renderer::RenderText("WORLD", projection, glm::vec3(windowSize.x - 500.0, windowSize.y - 50.0f, 0.3f), glm::vec2(0.7), { 1.0f, 1.0f, 1.0f, 1.0f });
    std::string levelString = std::to_string(s_Data.WorldLevel) + " - " + std::to_string(s_Data.WorldSubLevel);
    Renderer::RenderText(levelString, projection, glm::vec3(windowSize.x - 500.0, windowSize.y - 80.0f, 0.3f), glm::vec2(0.7), { 1.0f, 1.0f, 1.0f, 1.0f });

    // Time
    Renderer::RenderText("TIME", projection, glm::vec3(windowSize.x - 120.0, windowSize.y - 50.0f, 0.3f), glm::vec2(0.7), { 1.0f, 1.0f, 1.0f, 1.0f });
    Renderer::RenderText(std::to_string(s_Data.TimeLeft), projection, glm::vec3(windowSize.x - 120.0, windowSize.y - 80.0f, 0.3f), glm::vec2(0.7), { 1.0f, 1.0f, 1.0f, 1.0f });
}
