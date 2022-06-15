//
//  StartScreen.cpp
//  Mario
//
//  Created by iKan on 15/06/22.
//

#include "StartScreen.hpp"

using namespace Mario;

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

struct StartScreenRenderingData {
    std::shared_ptr<Texture> Sprite;
    std::unordered_map<char, std::shared_ptr<SubTexture>> TileMap;
    std::vector<std::shared_ptr<Entity>> Entities;
};
static StartScreenRenderingData* s_Data;

/// Initialize the background tiles and creat entities
/// @param scene Scene reference pointer
void StartScreen::Init(const std::shared_ptr<Scene>& scene) {
    // 1. Allocate the memory for StartScreenRenderingData
    s_Data = new StartScreenRenderingData();
    
    // 2. Sprite Texture tile
    s_Data->Sprite = Renderer::GetTexture(AssetManager::GetClientAsset("textures/StartScreenTile.png"), false, false);
    
    // 3. Initialise the tiles and map
    CreateSubtextures();
    
    // 4. Create Entities
    
    // Extract the map width. MAP Width should be same for each New line string
    size_t mapWidth    = s_MapTiles.find_first_of('0') + 1;
    uint32_t mapHeight = static_cast<uint32_t>(strlen(s_MapTiles.c_str())) / mapWidth;
    
    // Creating entity for each tiles
    IK_INFO("Creating Entity for each tiles");
    for (uint32_t y = 0; y < mapHeight; y++) {
        for (uint32_t x = 0; x < mapWidth; x++) {
            if (char tileType = s_MapTiles[x + y * mapWidth]; s_Data->TileMap.find(tileType) != s_Data->TileMap.end()) {
                auto entity = scene->CreateEntity();
                auto spriteEntity = entity->AddComponent<SpriteComponent>(s_Data->TileMap[tileType]);
                auto spriteSize   = spriteEntity.SubTexture->GetSpriteSize();
                
                auto& tc = entity->GetComponent<TransformComponent>();
                
                tc.Translation = { x + 8, (mapHeight / 2.0f) - y + 2, 0.0f };
                tc.Scale       = { spriteSize.x, spriteSize.y , 0.0f};
                
                // Store to delete in end
                s_Data->Entities.emplace_back(entity);
                
            } //if (char tileType = s_MapTiles[x + y * mapWidth]; s_TextureMap.find(tileType) != s_TextureMap.end())
        } // for (uint32_t x = 0; x < mapWidth; x++)
    } // for (uint32_t y = 0; y < mapHeight; y++)
}

/// Initialize the background tiles and creat entities
/// @param scene Scene reference pointer
void StartScreen::Shutdown(const std::shared_ptr<Scene>& scene) {
    for (auto& entity : s_Data->Entities)
        scene->DestroyEntity(entity);
    
    s_Data->Entities.clear();
    
    for (auto& tileMap : s_Data->TileMap) {
        tileMap.second.reset();
    }
    s_Data->TileMap.clear();
    
    s_Data->Sprite.reset();
}

/// Initialize the start screen tiles
void StartScreen::CreateSubtextures()
{
    for (size_t i = 0; i < 10; i++)
        s_Data->TileMap['a' + (char)i] = SubTexture::CreateFromCoords(s_Data->Sprite, { float(i), 0.0f }, { 1.0f, 1.0f }, { 8.0f, 8.0f });
    
    s_Data->TileMap['N'] = SubTexture::CreateFromCoords(s_Data->Sprite, { 11.0f, 0.0f }, { 1.0f, 1.0f }, { 8.0f, 8.0f });
    s_Data->TileMap['O'] = SubTexture::CreateFromCoords(s_Data->Sprite, { 12.0f, 0.0f }, { 1.0f, 1.0f }, { 8.0f, 8.0f });
    s_Data->TileMap['P'] = SubTexture::CreateFromCoords(s_Data->Sprite, { 13.0f, 0.0f }, { 1.0f, 1.0f }, { 8.0f, 8.0f });
    
    for (size_t i = 0; i < 13; i++)
        s_Data->TileMap['l' + (char)i] = SubTexture::CreateFromCoords(s_Data->Sprite, { float(i), 1.0f }, { 1.0f, 1.0f }, { 8.0f, 8.0f });
    
    for (size_t i = 0; i < 2; i++)
        s_Data->TileMap['y' + (char)i] = SubTexture::CreateFromCoords(s_Data->Sprite, { float(i), 2.0f }, { 1.0f, 1.0f }, { 8.0f, 8.0f });
    
    for (size_t i = 0; i < 13; i++)
        s_Data->TileMap['A' + (char)i] = SubTexture::CreateFromCoords(s_Data->Sprite, { float(i + 2), 2.0f }, { 1.0f, 1.0f }, { 8.0f, 8.0f });
    
    s_Data->TileMap[' '] = s_Data->TileMap['j'];
}
