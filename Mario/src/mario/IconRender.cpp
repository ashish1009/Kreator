//
//  IconRender.cpp
//  Mario
//
//  Created by iKan on 16/06/22.
//

#include "IconRender.hpp"

using namespace Mario;

/// Render Runtime Icon without camera view
/// @param projection camera projection
void IconRender::RunTimeIcon(const glm::mat4& projection) {
    static glm::mat4 t = Math::GetTransformMatrix(glm::vec3(-6.4f, 7.7f, 0.3f), glm::vec3(0.0f), glm::vec3(0.6f));
    static std::shared_ptr<Texture> sprite = Renderer::GetTexture(AssetManager::GetClientAsset("textures/MarioTile.png"), false, false);
    static std::shared_ptr<SubTexture> coinIcon = SubTexture::CreateFromCoords(sprite, { 24.0f, 26.0f });
    
    BatchRenderer::BeginBatch(projection, glm::mat4(1.0f));
    BatchRenderer::DrawQuad(t, coinIcon, -1);
    BatchRenderer::EndBatch();
}

/// Render Runtime Icon without camera view
/// @param projection camera projection
void IconRender::InitTimeIcon(const glm::mat4& projection, float yPos) {
    static glm::vec3 scale = glm::vec3(0.5f);
    static glm::vec3 rotation = glm::vec3(0.0f);
    static glm::vec3 position = glm::vec3(-3.5f, yPos, 0.3f);

    glm::mat4 t = Math::GetTransformMatrix(position, rotation, scale);
    static std::shared_ptr<Texture> selectPlayerTexture = Renderer::GetTexture(AssetManager::GetClientAsset("textures/SelectPlayer.png"), false, false);
    
    BatchRenderer::BeginBatch(projection, glm::mat4(1.0f));
    BatchRenderer::DrawQuad(t, selectPlayerTexture);
    BatchRenderer::EndBatch();
}
