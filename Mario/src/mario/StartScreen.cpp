//
//  StartScreen.cpp
//  Mario
//
//  Created by iKan on 13/06/22.
//

#include "StartScreen.hpp"

using namespace Mario;

std::shared_ptr<Texture> StartScreen::s_Sprite = nullptr;
std::shared_ptr<Texture> StartScreen::s_SelectePlayer = nullptr;
std::unordered_map<char, std::shared_ptr<SubTexture>> StartScreen::s_TileMap;
std::vector<std::shared_ptr<Entity>> StartScreen::s_Entities;

static std::string s_MapTiles =
//          I__K A N
"aggggggggggggggggggggb0"
"e    I  JJ  yzG      h0"
"e    w  JlyzJJJ      h0"
"e    w  JJJvJCH      h0"
"e    N  NNJJNOP      h0"
"e      yuzMwMzIyz    h0"
"e      JxxJvJlwww    h0"
"e      JJJNNJtwCD    h0"
"cffffffnnnffnnnEFffffd0"
;

/// Initialize the background tiles and creat entities
/// @param scene Scene reference pointer
void StartScreen::CreateEntities(const std::shared_ptr<Scene>& scene) {
    // Texture tile
    s_Sprite = Renderer::GetTexture(AssetManager::GetClientAsset("textures/StartScreenTile.png"), false, false);
    s_SelectePlayer = Renderer::GetTexture(AssetManager::GetClientAsset("textures/SelectPlayer.png"), false, false);
    
    // Initialise the tiles and map
    Init();
    
    // Extract the map width. MAP Width should be same for each New line string
    size_t mapWidth    = s_MapTiles.find_first_of('0') + 1;
    uint32_t mapHeight = static_cast<uint32_t>(strlen(s_MapTiles.c_str())) / mapWidth;
    
    // Creating entity for each tiles
    IK_INFO("Creating Entity for each tiles");
    for (uint32_t y = 0; y < mapHeight; y++) {
        for (uint32_t x = 0; x < mapWidth; x++) {
            if (char tileType = s_MapTiles[x + y * mapWidth]; s_TileMap.find(tileType) != s_TileMap.end()) {
                auto entity = scene->CreateEntity();
                auto spriteEntity = entity->AddComponent<SpriteComponent>(s_TileMap[tileType]);
                auto spriteSize   = spriteEntity.SubTexture->GetSpriteSize();
                
                auto& tc = entity->GetComponent<TransformComponent>();
                
                tc.Translation = { x + 8, (mapHeight / 2.0f) - y + 2, 0.0f };
                tc.Scale       = { spriteSize.x, spriteSize.y , 0.0f};
                
                s_Entities.emplace_back(entity);
                
            } //if (char tileType = s_MapTiles[x + y * mapWidth]; s_TextureMap.find(tileType) != s_TextureMap.end())
        } // for (uint32_t x = 0; x < mapWidth; x++)
    } // for (uint32_t y = 0; y < mapHeight; y++)
}

/// Initialize the background tiles and creat entities
/// @param scene Scene reference pointer
void StartScreen::DestroyEntities(const std::shared_ptr<Scene>& scene) {
    for (auto& entity : s_Entities)
        scene->DestroyEntity(entity);
    
    s_Entities.clear();
    
    for (auto& tileMap : s_TileMap) {
        tileMap.second.reset();
    }
    s_TileMap.clear();
    
    s_Sprite.reset();
}

void StartScreen::Update(const glm::mat4& projection, float yPos) {
    glm::mat4 t = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, yPos, 0.4f)) * glm::toMat4(glm::quat(glm::vec3(0.0f))) * glm::scale(glm::mat4(1.0f), glm::vec3(0.6f));
    
    BatchRenderer::BeginBatch(projection, glm::mat4(1.0f));
    BatchRenderer::DrawQuad(t, s_SelectePlayer);
    BatchRenderer::EndBatch();
}

/// Initialize the start screen tiles
void StartScreen::Init()
{
    for (size_t i = 0; i < 10; i++)
        s_TileMap['a' + (char)i] = SubTexture::CreateFromCoords(s_Sprite, { float(i), 0.0f }, { 1.0f, 1.0f }, { 8.0f, 8.0f });
    
    s_TileMap['N'] = SubTexture::CreateFromCoords(s_Sprite, { 11.0f, 0.0f }, { 1.0f, 1.0f }, { 8.0f, 8.0f });
    s_TileMap['O'] = SubTexture::CreateFromCoords(s_Sprite, { 12.0f, 0.0f }, { 1.0f, 1.0f }, { 8.0f, 8.0f });
    s_TileMap['P'] = SubTexture::CreateFromCoords(s_Sprite, { 13.0f, 0.0f }, { 1.0f, 1.0f }, { 8.0f, 8.0f });
    
    for (size_t i = 0; i < 13; i++)
        s_TileMap['l' + (char)i] = SubTexture::CreateFromCoords(s_Sprite, { float(i), 1.0f }, { 1.0f, 1.0f }, { 8.0f, 8.0f });
    
    for (size_t i = 0; i < 2; i++)
        s_TileMap['y' + (char)i] = SubTexture::CreateFromCoords(s_Sprite, { float(i), 2.0f }, { 1.0f, 1.0f }, { 8.0f, 8.0f });
    
    for (size_t i = 0; i < 13; i++)
        s_TileMap['A' + (char)i] = SubTexture::CreateFromCoords(s_Sprite, { float(i + 2), 2.0f }, { 1.0f, 1.0f }, { 8.0f, 8.0f });
    
    s_TileMap[' '] = s_TileMap['j'];
}

void StartScreen::RenderText(const glm::mat4& projection) {
    // Render the Frame rate
    Renderer::RenderText("(c)2022 iKan Mario", projection, glm::vec3(950.0f, 265.0f, 0.3f), glm::vec2(0.5), { 1.0f, 1.0f, 1.0f, 0.8f });

    Renderer::RenderText("1 PLAYER GAME", projection, glm::vec3(550.0f, 200.0f, 0.3f), glm::vec2(1.0), { 1.0f, 1.0f, 1.0f, 0.8f });
    Renderer::RenderText("2 PLAYER GAME", projection, glm::vec3(550.0f, 150.0f, 0.3f), glm::vec2(1.0), { 1.0f, 1.0f, 1.0f, 0.8f });

}
