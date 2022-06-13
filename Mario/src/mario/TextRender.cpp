//
//  TextRender.cpp
//  Mario
//
//  Created by iKan on 12/06/22.
//

#include "TextRender.hpp"

using namespace Mario;

static TextRenderData s_Data;

/// Initialize the Text Data
void TextRender::Init(const TextRenderData& textData) {
    s_Data.Score = textData.Score;
    s_Data.CoinCount = textData.CoinCount;
    s_Data.WorldLevel = textData.WorldLevel;
    s_Data.WorldSubLevel = textData.WorldSubLevel;
    s_Data.TimeLeft = textData.TimeLeft;
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
