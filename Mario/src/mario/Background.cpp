//
//  Background.cpp
//  Mario
//
//  Created by iKan on 08/06/22.
//

#include "Background.hpp"

using namespace Mario;

ThemeColor Background::CurrentTheme = ThemeColor::Brown;
std::shared_ptr<Texture> Background::Sprite;
std::unordered_map<std::shared_ptr<Entity>, std::shared_ptr<Background::Tile>> Background::TileEntityMap;
std::unordered_map<char, std::array<std::shared_ptr<SubTexture>, MAX_COLOR>> Background::TilesColorMapWithChar;
std::unordered_map<char, std::shared_ptr<Background::Tile>> Background::TileMap;

/// Get entity name from Char code
/// @param type Char of time map
static std::string GetEntityNameFromChar(char type) {
    switch(type) {
        case 'G' : return "Ground"; break;
            
        case '|' : return "Castel Brick";        break;
        case 'o' : return "Castel Gate";         break;
        case 'u' : return "castel Gate Domb";    break;
        case '.' : return "Castel Domb";         break;
        case 'l' : return "Castel Windlow Left"; break;
        case 'r' : return "Castel Window Right"; break;
            
        case 'S' : return "Steps";     break;
        case '-' : return "Bridge";    break;
        case '!' : return "Pipe Base"; break;
        case 'Y' : return "Pipe";      break;
        case 'X' : return "Bricks";    break;
        case 'B' : return "Bonus";     break;
            
        case '<' : return "Grass <";   break;
        case 'v' : return "Grass v";   break;
        case '>' : return "Grass >>";  break;
            
        case '(' : return "Cloud (";    break;
        case '^' : return "Cloud ^";    break;
        case ')' : return "Cloud )>";   break;
            
        case '}' : return "Grass }";    break;
        case '{' : return "Grass {";    break;
        case '*' : return "Grass *";    break;
        case '1' : return "Grass 1";    break;
        case '2' : return "Grass 2";    break;
        case '3' : return "Grass 3";    break;
    }
    IK_ASSERT(false, "Invalid Type");
    return "";
}

/// Initialize the Background data
/// @param scene scene reference pointer
void Background::Init(const std::shared_ptr<Scene>& scene) {
    IK_CLIENT_LOG_SEPARATOR();
    IK_INFO("Initializing the background data");

    Sprite = Renderer::GetTexture(AssetManager::GetClientAsset("textures/MarioTile.png"), false, false);
    
    // Initialize all Subtextures for all theme
    StoreSubtextureOfEachTile();
    
    // Extract the map width. MAP Width should be same for each New line string
    size_t mapWidth    = s_MapTiles.find_first_of('0') + 1;
    uint32_t mapHeight = static_cast<uint32_t>(strlen(s_MapTiles.c_str())) / mapWidth;

    IK_INFO("Creating Entity for each tile ");
    for (uint32_t y = 0; y < mapHeight; y++) {
        for (uint32_t x = 0; x < mapWidth; x++) {
            // Create entity if we have sub texture for the character we found in map
            if (char tileType = s_MapTiles[x + y * mapWidth]; TileMap.find(tileType) != TileMap.end()) {
                auto entity = scene->CreateEntity(GetEntityNameFromChar(tileType));
                const auto& spriteEntity = entity->AddComponent<SpriteComponent>(TileMap[tileType]->SubTexture);
                const auto& spriteSize   = spriteEntity.SubTexture->GetSpriteSize();

                auto& tc = entity->GetComponent<TransformComponent>();
                
                tc.Translation = { x, (mapHeight / 2.0f) - y, 0.0f };
                tc.Scale       = { spriteSize.x, spriteSize.y , 0.0f};
                
                // Store the each tile and entity map
                TileEntityMap[entity] = TileMap[tileType];
            }
            else {
                if (tileType != ' ' || tileType != '0') // No need to validate Space
                    IK_WARN("    Char {0} at position Row {1} and Column {2} is not found in Tile Map", tileType, y, x);
            }
        }
    }
}

/// shut down the Background
void Background::Shutdown() {
    for (auto tile : TileEntityMap) {
        tile.second.reset();
    }
    
    for (auto tile : TileMap) {
        tile.second.reset();
    }
        
    Sprite.reset();
}

/// Stores the subtexture from Sprite for each tile in init time
void Background::StoreSubtextureOfEachTile() {
    IK_INFO("    Storing the Subtextures for each Tile");

    // Ground SubTexutres
    {
        TilesColorMapWithChar['G'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 0.0f, 27.0f })); // Brown
        TilesColorMapWithChar['G'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 0.0f, 25.0f })); // Blue
        TilesColorMapWithChar['G'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 0.0f, 23.0f })); // Grey
        TilesColorMapWithChar['G'][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 0.0f, 21.0f })); // Green
        
        TileMap['G'] = Tile::Create(TilesColorMapWithChar['G'][CurrentTheme]);
    }
    
    // Bricks SubTextures
    {
        TilesColorMapWithChar['X'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 17.0f, 27.0f })); // Brown;
        TilesColorMapWithChar['X'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 17.0f, 25.0f })); // Blue;
        TilesColorMapWithChar['X'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 17.0f, 23.0f })); // Grey;
        TilesColorMapWithChar['X'][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 17.0f, 21.0f })); // green;
        
        TileMap['X'] = Tile::Create(TilesColorMapWithChar['X'][CurrentTheme]);
    }
    
    // Bonus SubTextures
    {
        // Unused bonus (from 0 -3)
        TilesColorMapWithChar['B'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 24.0f, 27.0f })); // Brown
        TilesColorMapWithChar['B'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 24.0f, 25.0f })); // Blue
        TilesColorMapWithChar['B'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 24.0f, 23.0f })); // Grey
        TilesColorMapWithChar['B'][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 24.0f, 21.0f })); // Green
        
        // USed bonus (from 4 -7)
        TilesColorMapWithChar['B'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 27.0f, 27.0f })); // Brown
        TilesColorMapWithChar['B'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 27.0f, 25.0f })); // Blue
        TilesColorMapWithChar['B'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 27.0f, 23.0f })); // Grey
        TilesColorMapWithChar['B'][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 27.0f, 21.0f })); // Green
        
        TileMap['B'] = Tile::Create(TilesColorMapWithChar['B'][CurrentTheme]);
    }
    
    // Steps SubTextures
    {
        TilesColorMapWithChar['S'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 0.0f, 26.0f })); // Brown
        TilesColorMapWithChar['S'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 0.0f, 24.0f })); // Blue
        TilesColorMapWithChar['S'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 0.0f, 22.0f })); // Grey
        TilesColorMapWithChar['S'][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 0.0f, 20.0f })); // Green
        
        TileMap['S'] = Tile::Create(TilesColorMapWithChar['S'][CurrentTheme]);
    }
    
    // Bridge SubTexture
    {
        TilesColorMapWithChar['-'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 15.0f, 18.5f })); // Green
        TilesColorMapWithChar['-'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 15.0f, 14.5f })); // Orange
        TilesColorMapWithChar['-'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 15.0f, 10.5f })); // Grey
        TilesColorMapWithChar['-'][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 15.0f,  8.5f })); // Pink
        
        TileMap['-'] = Tile::Create(TilesColorMapWithChar['-'][CurrentTheme]);
    }
    
    // Pipes
    {
        // Heads
        TilesColorMapWithChar['Y'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 0.0f, 19.0f }, { 2.0f, 1.0f })); // Green
        TilesColorMapWithChar['Y'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 0.0f, 15.0f }, { 2.0f, 1.0f })); // Orange
        TilesColorMapWithChar['Y'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 0.0f, 13.0f }, { 2.0f, 1.0f })); // Grey
        TilesColorMapWithChar['Y'][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 0.0f,  9.0f }, { 2.0f, 1.0f })); // Pink
        
        TileMap['Y'] = Tile::Create(TilesColorMapWithChar['Y'][CurrentTheme]);
        
        // Base
        TilesColorMapWithChar['!'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 0.0f, 18.0f }, { 2.0f, 1.0f })); // Green
        TilesColorMapWithChar['!'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 0.0f, 14.0f }, { 2.0f, 1.0f })); // Orange
        TilesColorMapWithChar['!'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 0.0f, 12.0f }, { 2.0f, 1.0f })); // Grey
        TilesColorMapWithChar['!'][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 0.0f,  8.0f }, { 2.0f, 1.0f })); // Pink
        
        TileMap['!'] = Tile::Create(TilesColorMapWithChar['!'][CurrentTheme]);
    }
    
    // Grass
    {
        TilesColorMapWithChar['<'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 11.0f, 18.0f })); // Green
        TilesColorMapWithChar['<'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 11.0f, 14.0f })); // Orange
        TilesColorMapWithChar['<'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 11.0f, 12.0f })); // Grey
        TilesColorMapWithChar['<'][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 11.0f,  8.0f })); // Pink
        
        TileMap['<'] = Tile::Create(TilesColorMapWithChar['<'][CurrentTheme], false);
        
        TilesColorMapWithChar['v'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 12.0f, 18.0f })); // Green
        TilesColorMapWithChar['v'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 12.0f, 14.0f })); // Orange
        TilesColorMapWithChar['v'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 12.0f, 12.0f })); // Grey
        TilesColorMapWithChar['v'][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 12.0f,  8.0f })); // Pink
        
        TileMap['v'] = Tile::Create(TilesColorMapWithChar['v'][CurrentTheme], false);
        
        TilesColorMapWithChar['>'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 13.0f, 18.0f })); // Green
        TilesColorMapWithChar['>'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 13.0f, 14.0f })); // Orange
        TilesColorMapWithChar['>'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 13.0f, 12.0f })); // Grey
        TilesColorMapWithChar['>'][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 13.0f, 8.0f  })); // Pink
        
        TileMap['>'] = Tile::Create(TilesColorMapWithChar['>'][CurrentTheme], false);
    }
    
    // Forest Grass
    {
        TilesColorMapWithChar['{'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 8.0f, 19.0f })); // Green
        TilesColorMapWithChar['{'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 8.0f, 15.0f })); // Orange
        TilesColorMapWithChar['{'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 8.0f, 13.0f })); // Grey
        TilesColorMapWithChar['{'][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 8.0f,  9.0f })); // Pink
        
        TileMap['{'] = Tile::Create(TilesColorMapWithChar['{'][CurrentTheme], false);
        
        TilesColorMapWithChar['}'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 10.0f, 19.0f })); // Green
        TilesColorMapWithChar['}'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 10.0f, 15.0f })); // Orange
        TilesColorMapWithChar['}'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 10.0f, 13.0f })); // Grey
        TilesColorMapWithChar['}'][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 10.0f,  9.0f })); // Pink
        
        TileMap['}'] = Tile::Create(TilesColorMapWithChar['}'][CurrentTheme], false);
        
        
        TilesColorMapWithChar['*'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 9.0f, 19.0f })); // Green
        TilesColorMapWithChar['*'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 9.0f, 15.0f })); // Orange
        TilesColorMapWithChar['*'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 9.0f, 13.0f })); // Grey
        TilesColorMapWithChar['*'][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 9.0f,  9.0f })); // Pink
        
        TileMap['*'] = Tile::Create(TilesColorMapWithChar['*'][CurrentTheme], false);
        
        TilesColorMapWithChar['1'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 8.0f, 18.0f })); // Green
        TilesColorMapWithChar['1'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 8.0f, 14.0f })); // Orange
        TilesColorMapWithChar['1'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 8.0f, 12.0f })); // Grey
        TilesColorMapWithChar['1'][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 8.0f,  8.0f })); // Pink
        
        TileMap['1'] = Tile::Create(TilesColorMapWithChar['1'][CurrentTheme], false);
        
        TilesColorMapWithChar['2'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 9.0f, 18.0f })); // Green
        TilesColorMapWithChar['2'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 9.0f, 14.0f })); // Orange
        TilesColorMapWithChar['2'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 9.0f, 12.0f })); // Grey
        TilesColorMapWithChar['2'][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 9.0f,  8.0f })); // Pink
        
        TileMap['2'] = Tile::Create(TilesColorMapWithChar['2'][CurrentTheme], false);
        
        TilesColorMapWithChar['3'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 10.0f, 18.0f })); // Green
        TilesColorMapWithChar['3'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 10.0f, 14.0f })); // Orange
        TilesColorMapWithChar['3'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 10.0f, 12.0f })); // Grey
        TilesColorMapWithChar['3'][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 10.0f,  8.0f })); // Pink
        
        TileMap['3'] = Tile::Create(TilesColorMapWithChar['3'][CurrentTheme], false);
    }
    
    // Cloud
    {
        TilesColorMapWithChar['('][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 0.0f, 0.0f }, { 1.0f, 2.0f })); // Red
        TilesColorMapWithChar['('][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 0.0f, 2.0f }, { 1.0f, 2.0f })); // Red
        TilesColorMapWithChar['('][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 0.0f, 0.0f }, { 1.0f, 2.0f })); // Grey
        TilesColorMapWithChar['('][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 0.0f, 2.0f }, { 1.0f, 2.0f })); // Blue
        
        TileMap['('] = Tile::Create(TilesColorMapWithChar['('][CurrentTheme], false);
        
        TilesColorMapWithChar['^'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 1.0f, 0.0f }, { 1.0f, 2.0f })); // Red
        TilesColorMapWithChar['^'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 1.0f, 2.0f }, { 1.0f, 2.0f })); // Red
        TilesColorMapWithChar['^'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 1.0f, 0.0f }, { 1.0f, 2.0f })); // Grey
        TilesColorMapWithChar['^'][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 1.0f, 2.0f }, { 1.0f, 2.0f })); // Blue
        
        TileMap['^'] = Tile::Create(TilesColorMapWithChar['^'][CurrentTheme], false);
        
        TilesColorMapWithChar[')'][ThemeColor::Brown] = (SubTexture::CreateFromCoords(Sprite, { 2.0f, 0.0f }, { 1.0f, 2.0f })); // Red
        TilesColorMapWithChar[')'][ThemeColor::Blue]  = (SubTexture::CreateFromCoords(Sprite, { 2.0f, 2.0f }, { 1.0f, 2.0f })); // Red
        TilesColorMapWithChar[')'][ThemeColor::Grey]  = (SubTexture::CreateFromCoords(Sprite, { 2.0f, 0.0f }, { 1.0f, 2.0f })); // Grey
        TilesColorMapWithChar[')'][ThemeColor::Green] = (SubTexture::CreateFromCoords(Sprite, { 2.0f, 2.0f }, { 1.0f, 2.0f })); // Blue
        
        TileMap[')'] = Tile::Create(TilesColorMapWithChar[')'][CurrentTheme], false);
    }
    
    // Castel
    {
        TileMap['.'] = Tile::Create(SubTexture::CreateFromCoords(Sprite, { 19.0f, 25.0f }), false);
        TileMap['u'] = Tile::Create(SubTexture::CreateFromCoords(Sprite, { 20.0f, 24.0f }), false);
        TileMap['o'] = Tile::Create(SubTexture::CreateFromCoords(Sprite, { 21.0f, 24.0f }), false);
        TileMap['|'] = Tile::Create(SubTexture::CreateFromCoords(Sprite, { 21.0f, 25.0f }), false);
        TileMap['l'] = Tile::Create(SubTexture::CreateFromCoords(Sprite, { 20.0f, 25.0f }), false);
        TileMap['r'] = Tile::Create(SubTexture::CreateFromCoords(Sprite, { 22.0f, 25.0f }), false);
    }
}
